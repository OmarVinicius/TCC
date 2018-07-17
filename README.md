# TCC

Algoritmo experimental baseado nas seguintes implementações:

- Criação de perfil de serviço Headset em um computador e respectiva simulação.
    Disponível em: http://www.drdobbs.com/mobile/using-bluetooth/232500828
    Autor: Ben DuPont

- Manipulação de sockets RFCOMM, L2CAP e SCO.
    Em: "Bluetooth ESSENTIALS FOR PROGRAMMERS"
    Autores: Albert S. Huang e Larry Rudolph
    
- Código-fonte da biblioteca BlueZ

Inclui a ferramenta bdaddr, para modificação do endereço MAC dos dispositivos Bluetooth, com instruções de compilação obtida em: http://blog.petrilopia.net/linux/change-your-bluetooth-device-mac-address/


Implementação testada em Ubuntu 16.04 com BlueZ 5.37.
Necessário o uso de 2 adaptadores Bluetooth com suporte ao emparelhamento SSP.

Instruções de uso:

- Instalar as bibliotecas libbluetooth, libpulse e libdbus:

    - sudo apt-get install libluetooth-dev
    - sudo apt-get install libpulse-dev
    - sudo apt-get install libdbus-1-dev

- Executar as seguintes alterações para que os serviços possam ser editados no SDP:

    - sudo gedit /etc/systemd/system/dbus-org.bluez.service
    - inserir argumento --compat em ExecStart=/usr/lib/bluetooth/bluetoothd
    - sudo systemctl daemon-reload
    - sudo systemctl restart bluetooth
    - sudo chmod 777 /var/run/sdp
    
- Tornar executável o script necessário a configuração e execução do algoritmo:

    - chmod a+rx script-tcc.sh

- Executá-lo:

    - sudo ./script-tcc.sh
    
- Na ferramenta bluetoothctl efetuar os seguintes passos:

    - Configurar o agente de conexão escolhido como padrão: default-agent
    - Aguardar a conexão do smartphone
    - Quando o smartphone conectar, autorizar todas as solicitações de serviços