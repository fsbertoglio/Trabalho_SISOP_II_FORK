#include "./subservices/monitoring_subservice.hpp"
#include "./subservices/interface_subservice.hpp"
#include "./subservices/discovery_subservice.hpp"
#include "./subservices/replica_subservice.hpp"
#include "global.hpp"

int main(int argc, char **argv)
{
    //  Gets local information
    string localMacAddress = getMacAddress();
    string localStatus = "awaken";
    string localIpAddress = getLocalIpAddress();
    string localHostName = gethostname();

    // Critical section for Table declaration
    participante *tabelaParticipantes = nullptr; // inicio da lista

    extern bool tabelaParticipantesUpdate;
    DiscoverySubservice discovery_obj(&tabelaParticipantesUpdate, localHostName, localIpAddress, localMacAddress, localStatus);
    if (discovery_obj.clientDiscoverySubservice() == 1)
    {
        sessionMode = "manager";
        MANAGER_IP_ADDRESS = localIpAddress;
    }
    else
    {
        sessionMode = "client";
    }

    // Critical section for TableUpdate declaration
    cout << "\n";
    novoParticipante(tabelaParticipantes, localHostName, localIpAddress, localMacAddress, localStatus);

    InterfaceSubservice interface(tabelaParticipantes, &tabelaParticipantesUpdate);
    MonitoringSubservice monitoring_obj(tabelaParticipantes, &tabelaParticipantesUpdate, localHostName, localIpAddress, localMacAddress, localStatus, sessionMode);
    ReplicaSubservice replica_obj(tabelaParticipantes, &tabelaParticipantesUpdate, localHostName, localIpAddress, localMacAddress, localStatus, sessionMode);

    bool isRunning = true;
    while (isRunning == true)
    {
        if (sessionMode == "manager")
        {
            thread int_thr([&interface]()
                           { interface.updateServerScreen(); });
            thread dsc_thr([&discovery_obj, &tabelaParticipantes]()
                           { discovery_obj.serverDiscoverySubservice(tabelaParticipantes); });
            thread mon_thr([&monitoring_obj]()
                           { monitoring_obj.serverMonitoringSubservice(); });
            thread rep_thr([&replica_obj]()
                           { replica_obj.serverReplicaSubservice(); });

            int_thr.join();
            dsc_thr.join();
            mon_thr.join();
            rep_thr.join();

            return 0;
        }
        else if (sessionMode == "client")
        {
            thread int_thr([&interface]()
                           {
            interface.setActive();
            interface.updateClientScreen(); });
            thread mon_thr([&monitoring_obj]()
                           { monitoring_obj.clientMonitoringSubservice(); });
            thread rep_thr([&replica_obj]()
                           { replica_obj.clientReplicaSubservice(); });

            int_thr.join();
            mon_thr.join();
            rep_thr.join();
            return 0;

        }
    }

    return 0;
}
