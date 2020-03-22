//
//  _____           _       _     ______                                                           _   _            _                ___       _           _   _    _____       _       _   _
// /  ___|         (_)     | |    | ___ \                                                         | | | |          | |              / _ \     | |         | | | |  /  ___|     | |     | | (_)
// \ `--.  ___ _ __ _ _ __ | |_   | |_/ / __ ___  _ __   ___  ___  ___ _ __    _ __   __ _ _ __   | | | | ___  _ __| |_ _____  __  / /_\ \_ __| | ________| | | |  \ `--.  ___ | |_   _| |_ _  ___  _ __
//  `--. \/ __| '__| | '_ \| __|  |  __/ '__/ _ \| '_ \ / _ \/ __|/ _ \ '__|  | '_ \ / _` | '__|  | | | |/ _ \| '__| __/ _ \ \/ /  |  _  | '__| |/ /______| | | |   `--. \/ _ \| | | | | __| |/ _ \| '_ \ 
// /\__/ / (__| |  | | |_) | |_   | |  | | | (_) | |_) | (_) \__ \  __/ |     | |_) | (_| | |     \ \_/ / (_) | |  | ||  __/>  <   | | | | |  |   <       \ \_/ /  /\__/ / (_) | | |_| | |_| | (_) | | | |
// \____/ \___|_|  |_| .__/ \__|  \_|  |_|  \___/| .__/ \___/|___/\___|_|     | .__/ \__,_|_|      \___/ \___/|_|   \__\___/_/\_\  \_| |_/_|  |_|\_\       \___/   \____/ \___/|_|\__,_|\__|_|\___/|_| |_|
//                   | |                         | |                          | |
//                   |_|                         |_|                          |_|

// Librairie Externe
#include "CTBot.h"
#include <WiFiUdp.h>
#include <WakeOnLan.h>
#include <ESP8266Ping.h>

#define LIGHT_ON_CALLBACK  "lightON"
#define LIGHT_OFF_CALLBACK "lightOFF"
// Variables
CTBot myBot;
WiFiUDP UDP;
WakeOnLan WOL(UDP);
CTBotInlineKeyboard buildinledkeyboard;
CTBotReplyKeyboard wolkeyboard;
bool isKeyboardActive;
String ssid = "Delta-Home-Network";
String pass = "Deltatech2814";
String token = "984300796:AAHbKkL9XJm6fnG93bFbT2-eeUEg8dfLAf4";
//Variables Ping
const IPAddress remote_ipnas(192, 168, 1, 144);
const IPAddress remote_ipworkstation(192, 168, 1, 3);
const IPAddress remote_iprxrack(192, 168, 1, 16);
const IPAddress remote_ipOLDHP(192, 168, 1, 18);
const IPAddress remote_ipoldhp(192, 168, 1, 60);
String machinestatus;
// Fonction Ping
void ping() {
  if (Ping.ping(remote_ipnas)) {
    machinestatus = "NAS: Allumer\n";
  }
  else {
    machinestatus = "NAS:  Éteint\n";
  }
  if (Ping.ping(remote_ipworkstation)) {
    machinestatus = machinestatus + "Workstation: Allumer\n";
  }
  else {
    machinestatus = machinestatus + "Workstation: Éteint\n";
  }
  if (Ping.ping(remote_iprxrack)) {
    machinestatus = machinestatus + "Rx-Server: Allumer\n";
  }
  else {
    machinestatus = machinestatus + "Rx-Server: Éteint\n";
  }

  if (Ping.ping(remote_ipOLDHP) || Ping.ping(remote_ipoldhp)) {
    machinestatus = machinestatus + "OLDHP: Allumer";
  }
  else {
    machinestatus = machinestatus + "OLDHP: Éteint";
  }
}
// Fonctions Allumer par WOL
void wakeMyWorkstation() {
  const char *MACAddress = "00:25:64:b2:d5:dc";
  WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
  // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
}
void wakeMyNAS() {
  const char *MACAddress = "14:da:e9:d6:2e:a7";
  WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
  // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
}
void wakeMyRXServer() {
  const char *MACAddress = "00:0a:e4:87:c0:cd";
  WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
  //MACAddress = "00:0a:e4:87:c0:cd";
  //WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
  // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
}
void wakeMyOLDHP() {
  const char *MACAddress = "00:1d:92:68:45:4d";
  WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
  MACAddress = "00:e0:53:41:37:35";
  WOL.sendMagicPacket(MACAddress); // Send Wake On Lan packet with the above MAC address. Default to port 9.
  // WOL.sendMagicPacket(MACAddress, 7); // Change the port number
}
// Fonction Setup
void setup() {
  WOL.setRepeat(3, 100);
  uint8_t telegramFingerprint [20] = { 0xBB, 0xDC, 0x45, 0x2A, 0x07, 0xE3, 0x4A, 0x71, 0x33, 0x40, 0x32, 0xDA, 0xBE, 0x81, 0xF7, 0x72, 0x6F, 0x4A, 0x2B, 0x6B };
  myBot.setFingerprint(telegramFingerprint);
  Serial.begin(115200);
  Serial.println("Starting TelegramBot...");
  myBot.setMaxConnectionRetries(0);
  myBot.setIP("192.168.1.195", "192.168.1.254", "255.255.255.0", "192.168.1.254", "1.1.1.1");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  myBot.setStatusPin(2);
  myBot.enableUTF8Encoding(true);
  myBot.useDNS(true);
  if (myBot.testConnection())
    Serial.println("\ntest de connexion réussi");
  else
    Serial.println("\nPas de connexion reseau");

  // Initialisation des ports
  pinMode(LED_BUILTIN, OUTPUT);

  // LED keyboard
  buildinledkeyboard.addButton("buildin_led_ON", LIGHT_ON_CALLBACK, CTBotKeyboardButtonQuery);
  buildinledkeyboard.addButton("buildin_led_OFF", LIGHT_OFF_CALLBACK, CTBotKeyboardButtonQuery);

  // WOL keyboard
  wolkeyboard.addButton("T7500_Wake_up");
  wolkeyboard.addButton("OLDHP_wake_up");
  wolkeyboard.addRow();
  wolkeyboard.addButton("NAS_wake_up");
  wolkeyboard.addButton("RX_wake_up");
  wolkeyboard.enableResize();
  isKeyboardActive = false;
}
void loop() {
  TBMessage msg;
  if (myBot.getNewMessage(msg)) {
    if (msg.sender.id == 575792002) {
      if (msg.messageType == CTBotMessageText) {
        if (msg.text.equalsIgnoreCase("/start")) {
          String reply;
          reply = (String)"Bienvenue " + msg.sender.username + (String)", \nEssayez les commandes suivantes: \n /status_buildin_led \n /buildin_led \n /wol \n /machines_status";
          myBot.sendMessage(msg.sender.id, reply);
        }
        else if (msg.text.equalsIgnoreCase("/wol")) {
          myBot.sendMessage(msg.sender.id, "Menu WAKE ON LAN", wolkeyboard);
          isKeyboardActive = true;
        }
        else if (msg.text.equalsIgnoreCase("T7500_Wake_up")) {
          wakeMyWorkstation();
          myBot.sendMessage(msg.sender.id, "La Workstation est en train de démarrer");
        }
        else if (msg.text.equalsIgnoreCase("NAS_wake_up")) {
          wakeMyNAS();
          myBot.sendMessage(msg.sender.id, "Le serveur Asustek-NAS est en train de démarrer");
        }
        else if (msg.text.equalsIgnoreCase("RX_wake_up")) {
          wakeMyRXServer();
          myBot.sendMessage(msg.sender.id, "Le serveur RX-rack est en train de démarrer");
        }
        else if (msg.text.equalsIgnoreCase("OLDHP_wake_up")) {
          wakeMyOLDHP();
          myBot.sendMessage(msg.sender.id, "Le serveur OLDHP est en train de démarrer");
        }
        else if (msg.text.equalsIgnoreCase("/status_buildin_led")) {
          isKeyboardActive = false;
          if (digitalRead(LED_BUILTIN) == HIGH) {
            myBot.sendMessage(msg.sender.id, "La LED est Eteinte");
          }
          else if (digitalRead(LED_BUILTIN) == LOW) {
            myBot.sendMessage(msg.sender.id, "La LED est Allumer");
          }
        }
        else if (msg.text.equalsIgnoreCase("/buildin_led")) {
          myBot.sendMessage(msg.sender.id, "Menu de la LED intégrer", buildinledkeyboard);
          isKeyboardActive = true;
        }
        else if (msg.text.equalsIgnoreCase("/machines_status")) {
          isKeyboardActive = false;
          myBot.sendMessage(msg.sender.id, "Test en cours veuillez patienter");
          ping();
          myBot.sendMessage(msg.sender.id, machinestatus);
        }
        else {
          isKeyboardActive = false;
          myBot.sendMessage(msg.sender.id, "Erreur \nEssayer les commandes suivantes \n /status_buildin_led \n /buildin_led \n /wol");
        }
      }
      else if (msg.messageType == CTBotMessageQuery) {
        // received a callback query message
        if (msg.callbackQueryData.equals(LIGHT_ON_CALLBACK)) {
          // pushed "LIGHT ON" button...
          digitalWrite(LED_BUILTIN, LOW); // ...turn on the LED (inverted logic!)
          // terminate the callback with an alert message
          myBot.endQuery(msg.callbackQueryID, "Light on");
        } else if (msg.callbackQueryData.equals(LIGHT_OFF_CALLBACK)) {
          // pushed "LIGHT OFF" button...
          digitalWrite(LED_BUILTIN, HIGH); // ...turn off the LED (inverted logic!)
          // terminate the callback with a popup message
          myBot.endQuery(msg.callbackQueryID, "Light off");
        }
      }
    }
    else if (msg.sender.id == 779407491) {
      isKeyboardActive = false;
      myBot.sendMessage(msg.sender.id, "Coucou Alix Contacte moi a @DeltaWhiplash pour pouvoir ajouter des fonctions à mon bot");
    }
    else {
      isKeyboardActive = false;
      myBot.sendMessage(msg.sender.id, "Vous n'etes pas autorisé a utiliser ce bot");
    }
  }
  delay(500);
}
