Commandes pour Windows

gcc -I"CHEMIN/pd" -Wall -W -mms-bitfields -o horloge.o -c horloge.c

gcc -s shared -Wl, --enable-auto-import -L"CHEMIN/pd/bin" -lpd -lwsock32 -lkernel32 -luser32 lgdi32 -liberty -o horloge.dll horloge.o