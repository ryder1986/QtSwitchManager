#include "Snmp_lsm.h"
#include "PortFrame.h"

Snmp_Lsm::Snmp_Lsm(int dwRetries, int dwTimeout, u_short wAgentPort, const char *pcCommunity, u_short wManagerPort)
{
    m_wAgentPort = wAgentPort;
    m_strCommunity = pcCommunity;
    m_ctarget.set_version(version2c);         // set the SNMP version SNMPV1 or V2
    m_ctarget.set_retry(dwRetries);           // set the number of auto retries
    m_ctarget.set_timeout(dwTimeout);         // set timeout
    m_ctarget.set_readcommunity(m_strCommunity.toLocal8Bit().data());
    Snmp::socket_startup();  // Initialize socket subsystem
    StartTrap(wManagerPort);

}

int Snmp_Lsm::SnmpGet(QString &strRet, char *ip, char *oid_array)
{
    //----------[ create a SNMP++ session ]-----------------------------------
    UdpAddress address(ip);      // make a SNMP++ Generic address
    if ( !address.valid()) {           // check validity of address
        std::cout << "Invalid Address or DNS Name, " << ip << "\n";
        return -1;
    }// bind to any port and use IPv6 if needed
    int status;
    Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));
    if ( status != SNMP_CLASS_SUCCESS) {
        std::cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
        return -1;
    }

    //--------[ build up SNMP++ object needed ]-------------------------------
    Pdu pdu;                               // construct a Pdu object
    Vb vb;                                 // construct a Vb object

    //---------[ make Oid object to retrieve ]-------------------------------
    Oid oid= oid_array;
    if (!oid.valid())            // check validity of user oid
    {
        std::cout << "Invalid Oid, " << oid_array << "\n";
        return -1;
    }
    vb.set_oid(oid);                       // set the Oid portion of the Vb
    pdu += vb;

    address.set_port(m_wAgentPort);
    m_ctarget.set_address(address);

    //-------[ issue the request, blocked mode ]-----------------------------
    //std::cout << community.get_printable() << std::endl << std::flush;
    status = snmp.get( pdu, m_ctarget);
    if (status == SNMP_CLASS_SUCCESS)
    {
        for (int i = 0; i < pdu.get_vb_count(); i++)
        {
            pdu.get_vb(vb, i);

            std::cout << "**************************" << std::endl;
            std::cout << "VB nr: " << i << std::endl;
            std::cout << "Oid = " << vb.get_printable_oid() << std::endl;
            std::cout<< "Value = " << vb.get_printable_value() << std::endl;
            std::cout << "Syntax = " << vb.get_syntax() << std::endl;

            if ((vb.get_syntax() == sNMP_SYNTAX_ENDOFMIBVIEW) ||
                    (vb.get_syntax() == sNMP_SYNTAX_NOSUCHINSTANCE) ||
                    (vb.get_syntax() == sNMP_SYNTAX_NOSUCHOBJECT))
            {
                std::cout << "Exception: " << vb.get_syntax() << " occured." << std::endl;
            }
            strRet = vb.get_printable_value();
        }
    }
    else
    {
        std::cout << "SNMP++ Get Error, " << snmp.error_msg( status)
                  << " (" << status <<")" << std::endl;
        return -1;
    }

    return 0;
}

int Snmp_Lsm::SnmpWalk(QMap<QString, QString> &map_SnmpWalkRet, char *ip, char *pcOid)
{
    int requests = 0;        // keep track of # of requests
    int objects  = 0;

    //---------[ make a GenAddress and Oid object to retrieve ]---------------
    UdpAddress address(ip);      // make a SNMP++ Generic address
    if (!address.valid()) {           // check validity of address
        std::cout << "Invalid Address or DNS Name, " << ip << "\n";
        return -1;
    }
    Oid oid=pcOid;
    if (!oid.valid()) {            // check validity of user oid
        std::cout << "Invalid Oid, " << pcOid << "\n";
        return -1;
    }

    //---------[ determine options to use ]-----------------------------------
    bool subtree = false;

    //----------[ create a SNMP++ session ]-----------------------------------
    int status;
    // bind to any port and use IPv6 if needed
    Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

    if (status != SNMP_CLASS_SUCCESS) {
        std::cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
        return -1;
    }

    //--------[ build up SNMP++ object needed ]-------------------------------
    Pdu pdu;                               // construct a Pdu object
    Vb vb;                                 // construct a Vb object
    vb.set_oid(oid);                      // set the Oid portion of the Vb
    pdu += vb;                             // add the vb to the Pdu

    address.set_port(m_wAgentPort);
    m_ctarget.set_address(address);

    //-------[ issue the request, blocked mode ]-----------------------------
    while ((status = snmp.get_bulk(pdu, m_ctarget, 0, BULK_MAX))== SNMP_CLASS_SUCCESS) {
        requests++;
        std::cout << "request"<<requests <<std::endl;
        for (int z=0;z<pdu.get_vb_count(); z++) {
            pdu.get_vb(vb,z);

            Oid tmp;
            vb.get_oid(tmp);
            if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
            {
                std::cout << "End of SUBTREE Reached\n";
                std::cout << "Total # of Requests = " << requests << "\n";
                std::cout << "Total # of Objects  = " << objects  << "\n";
                return -4;
            }
            objects++;
            // look for var bind exception, applies to v2 only
            if (vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW) {
                std::cout <<"-----------------"<<std::endl<< vb.get_printable_oid() << " = ";
                std::cout << vb.get_printable_value() << "\n";
                map_SnmpWalkRet[vb.get_printable_oid()] = vb.get_printable_value();
            }
            else {
                std::cout << "End of MIB Reached\n";
                std::cout << "Total # of Requests = " << requests << "\n";
                std::cout << "Total # of Objects  = " << objects  << "\n";
                return -4;
            }
        }
        // last vb becomes seed of next rquest
        pdu.set_vblist(&vb, 1);
    }
    if (status != SNMP_ERROR_NO_SUCH_NAME)
    {
        std::cout << "SNMP++ snmpWalk Error, " << snmp.error_msg(status) << "\n";
    }
    std::cout << "Total # of Requests = " << requests << "\n";
    std::cout << "Total # of Objects  = " << objects  << "\n";

    return 0;
}

extern Snmp_Lsm *gptSnmp_Lsm;
void callback(int reason, Snmp *snmp, Pdu &pdu, SnmpTarget &target, void *cd)
{
    Vb nextVb;
    int pdu_error;
    GenAddress addr;

    target.get_address(addr);
    UdpAddress from(addr);

    std::cout << "reason: " << reason << std::endl
              << "msg: " << snmp->error_msg(reason) << std::endl
              << "from: " << from.get_printable() << std::endl;

    pdu_error = pdu.get_error_status();
    if (pdu_error){
        std::cout << "Response contains error: "
                  << snmp->error_msg(pdu_error)<< std::endl;
    }
    Oid id;
    pdu.get_notify_id(id);
    std::cout << "ID:  " << id.get_printable() << std::endl;
    std::cout << "Type:" << pdu.get_type() << std::endl;
    int i = 0;
    bool bSuccess = false;
    for (; i<pdu.get_vb_count(); i++)
    {
        pdu.get_vb(nextVb, i);
        const char *pcOid = nextVb.get_printable_oid();
        const char *pcVal = nextVb.get_printable_value();
        std::cout << "Oid: " << pcOid << std::endl
                  << "Val: " <<  pcVal << std::endl;
        if (PortFrame::SetInfoByOid(pcOid, pcVal) && !bSuccess)
        {
            bSuccess = true;
        }
        gptSnmp_Lsm->EmitWarnTextSignal(pcOid, pcVal);
    }

    if (bSuccess)
    {
        gptSnmp_Lsm->EmitPortWarnSignal();
    }

    if (pdu.get_type() == sNMP_PDU_INFORM)
    {
        std::cout << "pdu type: " << pdu.get_type() << std::endl;
        std::cout << "sending response to inform: " << std::endl;
        nextVb.set_value("This is the response.");
        pdu.set_vb(nextVb, 0);
        snmp->response(pdu, target);
    }

    std::cout << std::endl;
}

void Snmp_Lsm::EmitPortWarnSignal()
{
//    emit signal_SendPortWarn();
}

void Snmp_Lsm::EmitWarnTextSignal(QString strOid, QString strMsg)
{
  //  emit signal_SendWarnText(strOid, strMsg);
}

int Snmp_Lsm::StartTrap(int dwManagerPort)
{
    //----------[ create a SNMP++ session ]-----------------------------------
    int status;
    m_TrapSnmp = new Snmp(status);
    if (status != SNMP_CLASS_SUCCESS)
    {
        std::cout << "SNMP++ Session Create Fail, " << m_TrapSnmp->error_msg(status) << "\n";
        return 1;
    }

    OidCollection oidc;
    TargetCollection targetc;
    std::cout << "Trying to register for traps on port " << dwManagerPort << "." << std::endl;
    m_TrapSnmp->notify_set_listen_port(dwManagerPort);
    status = m_TrapSnmp->notify_register(oidc, targetc, callback, NULL);
    if (status != SNMP_CLASS_SUCCESS)
    {
        std::cout << "Error register for notify (" << status << "): "<< m_TrapSnmp->error_msg(status) << std::endl;
        exit(1);
    }
    else
        std::cout << "Waiting for traps/informs..." << std::endl;
    m_TrapSnmp->start_poll_thread(1000);

}

Snmp_Lsm::~Snmp_Lsm()
{
    m_TrapSnmp->stop_poll_thread();
    Snmp::socket_cleanup();  // Shut down socket subsystem
}
