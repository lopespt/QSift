#include <VideoAsift.hpp>

int main(int argc, char const *argv[])
{
    VideoASift vid("/home/lopespt/Downloads/v1/Conversor-",0,0);
    vid.extractASiftFeatures(0,100,5);
    
    return 0;
}
