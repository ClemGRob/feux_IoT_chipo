# feux_IoT_chipo


coté PC : 
sous Ubuntu 18.04 (autres versions non testés)
dans un premier terminal : 
    nmcli dev wifi hotspot ifname wlp3s0f0 ssid "Mooa" password "okokokok"
    sudo systemctl restart mosquitto
    mosquitto -v

dans un second : 

./Makefile.sh
./main.c


coté arduino
televerser le code organisation.ino
