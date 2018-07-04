# TCC

Algoritmo baseado nas seguintes implementações:

- Criação de perfil de serviço Headset em um computador e respectiva simulação.
    Disponível em: http://www.drdobbs.com/mobile/using-bluetooth/232500828
    Autor: Ben DuPont

- Manipulação de sockets RFCOMM, L2CAP e SCO.
    Em: "Bluetooth ESSENTIALS FOR PROGRAMMERS"
    Autores: Albert S. Huang e Larry Rudolph
    
- Código-fonte da biblioteca BlueZ

Implementação testada em Ubuntu 16.04 com BlueZ 5.37.
Necessário o uso de 2 adaptadores Bluetooth com SSP.

Instruções de compilação:

- Instalar as bibliotecas libbluetooth, libpulse e libdbus:

    - sudo apt-get install libluetooth-dev
    - sudo apt-get install libpulse-dev
    - sudo apt-get install libdbus-1-dev

- Compilar:
    
    - gcc main.c gerenc_adaptador.c servicos.c sockets.c -I "/usr/include/dbus-1.0/" -I "/usr/lib/x86_64-linux-gnu/dbus-1.0/include/" -L `pkg-config --cflags --libs libpulse-simple` -L `pkg-config --cflags --libs dbus-1` -lbluetooth -o tcc

Instruções de uso geral:

- Executar as seguintes alterações para que os serviços possam ser editados no SDP:

    - sudo gedit /etc/systemd/system/dbus-org.bluez.service
    - inserir argumento --compat em ExecStart=/usr/lib/bluetooth/bluetoothd
    - sudo systemctl daemon-reload
    - sudo systemctl restart bluetooth
    - sudo chmod 777 /var/run/sdp
    
- Ligar todos os adaptadores:

    - sudo hciconfig hci<índice_adaptador> up

- Através do btmgmt, desativar as configurações de Secure Connections e/ou Low Energy, de cada adaptador,
e habilitar o modo connectable e discoverable no adaptador de índice 0.

- Através do sdptool, excluir todos os registros de perfis de serviço Headset Audio Gateway:

    - sudo sdptool browse local
    - sudo sdptool del <Service_RecHandle>

- Executar o comando para reparo do bug de roteamento da SCO nos adaptadores que sejam Broadcom BCM20702:

    - sudo hcitool -i hci<índice_adaptador> cmd 0x3F 0x01C 0x01 0x02 0x00 0x01 0x01
    
- Buscar endereço MAC do headset:

    - sudo hcitool scan
    
- Executar o algoritmo:

    - sudo ./tcc <endereço_MAC_headset>
    
- Configurar o pareamento Just Works do adaptador (índice 1) que irá conectar-se ao headset, e aguardar conexão do smartphone e aceitar, via bluetoothctl:

    - sudo bluetoothctl
    - select <endereço_MAC_do_adaptador_índice_1>
    - agent NoInputNoOutput
    - default-agent
    - select <endereço_MAC_do_adaptador_índice_0>
    - agent NoInputNoOutput
    - default-agent
    - yes para as requisições de serviço
    - trust <endereço_MAC_do_smartphone>