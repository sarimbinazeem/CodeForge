#include "API/Server.h"
using namespace std;

int main()
{
    CodeForgeSystem codeForge;

    // Load saved problems and submissions
    codeForge.initialize();

    // Start the API server / feature runner
    APIServer api(codeForge);
    api.start();

    return 0;
}
