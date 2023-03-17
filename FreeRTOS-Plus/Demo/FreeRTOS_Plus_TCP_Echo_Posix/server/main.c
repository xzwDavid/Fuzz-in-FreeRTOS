#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

void vTCPServerTask(void *pvParameters) {
    (void)pvParameters;

    // 创建套接字
    Socket_t xListeningSocket = FreeRTOS_socket(FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP);
    configASSERT(xListeningSocket != FREERTOS_INVALID_SOCKET);

    // 绑定 IP 地址和端口号
    struct freertos_sockaddr xBindAddress;
    xBindAddress.sin_port = FreeRTOS_htons(8080);
    xBindAddress.sin_addr = FreeRTOS_inet_addr("0.0.0.0");
    configASSERT(FreeRTOS_bind(xListeningSocket, &xBindAddress, sizeof(xBindAddress)) == 0);

    // 监听连接
    configASSERT(FreeRTOS_listen(xListeningSocket, 1) == 0);

    while (1) {
        // 等待客户端连接
        struct freertos_sockaddr xClientAddress;
        socklen_t xClientAddressLength = sizeof(xClientAddress);
        Socket_t xConnectedSocket = FreeRTOS_accept(xListeningSocket, &xClientAddress, &xClientAddressLength);

        if (xConnectedSocket != FREERTOS_INVALID_SOCKET) {
            // 接收客户端发送的数据并原样返回
            uint8_t ucBuffer[128];
            while (1) {
                int iReceivedBytes = FreeRTOS_recv(xConnectedSocket, ucBuffer, sizeof(ucBuffer), 0);
                if (iReceivedBytes <= 0) {
                    break;
                }

                FreeRTOS_send(xConnectedSocket, ucBuffer, iReceivedBytes, 0);
            }

            // 关闭连接
            FreeRTOS_closesocket(xConnectedSocket);
        }
    }
}
