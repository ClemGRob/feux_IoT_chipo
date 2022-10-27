gcc -Wall mqtt_test/mqtt_pub.c -o pub.exe -lmosquitto
gcc -Wall mqtt_test/mqtt_sub.c -o sub.exe -lmosquitto
gcc -Wall mqtt_test/sub_test.c -o test.exe -lmosquitto
gcc -Wall tmpcode/thread.c -o thread.exe -lpthread
