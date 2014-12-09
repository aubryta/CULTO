set filename=%1

gcc -I"D:\Pd-0.43.4-extended" -Wall -W -mms-bitfields -o %filename%.o -c %filename%.c 

gcc -s -shared --enable-auto-import -L"D:\Pd-0.43.4-extended\bin" -lpd -lwsock32 -lkernel32 -luser32 -lgdi32 -o %filename%.dll %filename%.o
