#include "api/ApiServer.h"

int main()
{
    EduManage::ApiServer server;
    server.run(8080);
    return 0;
}
