#include <windows.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVICE_NAME "AutoEmacs"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ServiceMain(int argc, char **argv);

void ControlHandler(DWORD request);

int InitService();

int startEmacs();

int stopEmacs();

int main() {
//    stopEmacs();
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = SERVICE_NAME;
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION) ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

    // 启动服务的控制分派机线程
    StartServiceCtrlDispatcher(ServiceTable);
    // startEmacs();
}

void ServiceMain(int argc, char **argv) {
    int error;

    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 10;
    ServiceStatus.dwWaitHint = 10;

    hStatus = RegisterServiceCtrlHandler(SERVICE_NAME, (LPHANDLER_FUNCTION) ControlHandler);
    if (hStatus == (SERVICE_STATUS_HANDLE) 0) {
        // 服务注册失败, 直接返回
        return;
    }
    // 服务初始化
    error = InitService();
    if (error == -1) {
        // 初始化失败，终止服务
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode = -1;
        SetServiceStatus(hStatus, &ServiceStatus);
        // 退出 ServiceMain
        return;
    }
    // 向 SCM 报告运行状态
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);
}

int InitService() {
    int result;
    result = startEmacs();
    return (result);
}

void ControlHandler(DWORD request) {
    switch (request) {
        case SERVICE_CONTROL_STOP:
            stopEmacs();

            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(hStatus, &ServiceStatus);
            return;

        case SERVICE_CONTROL_SHUTDOWN:
            stopEmacs();

            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        default:
            break;
    }

    // 向 SCM 报告运行状态
    SetServiceStatus(hStatus, &ServiceStatus);
}

int startEmacs() {
    int pid = system("runemacs --daemon");
    if (pid < 0) {
        return -1;
    }
    return pid;
}

int stopEmacs() {
    int pid = system("emacsclientw --eval \"(kill-emacs)\"");
    if (pid < 0) {
        return -1;
    }
    return pid;
}
