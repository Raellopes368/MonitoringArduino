# Arduino GPS/GSM

## Descrição

Código para uso dos módulos gsm e gps, para monitoramento remoto da localização em tempo real.
O módulo Gps, busca os dados de latitude e longitude e o módulo gps os envia para uma api informada na linha 15 do código, com isso, você poderá salvar esses dados em um banco de dados, servir para um front end ou app mobile em tempo real.


## Instalação

Faça download do projeto zipado, ou dê um 

```git clone https://github.com/Raellopes368/MonitoringArduino.git```  com https informando usuário e senha, ou 

```git clone git@github.com:Raellopes368/MonitoringArduino.git``` via ssh.


instale a Biblioteca 
#### TinyGPS.h 
no link https://drive.google.com/file/d/0B91L9po09u35NU9UdXRBYS1OTXM/view




## Montagem

O módulo GPS, está configurado na linha 7, faça a montagem de acordo com sua definição dos pinos RX e TX no Arduino, lembre-se que o pino RX do módulo deve ir ligado ao pino TX do arduino (TX configurado no SoftSerial) e vice versa, o mesmo acontece para o módulo Gsm configurado na linha 20