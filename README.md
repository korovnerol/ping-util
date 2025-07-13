# ping_mac — Утилита для получения MAC-адреса по IPv4 через ICMP Echo Request

Этот инструмент позволяет отправить ICMP Echo Request (ping) на указанный IPv4-адрес и получить MAC-адрес устройства, ответившего на запрос.

- Отправка ICMPv4 Echo Request через raw-сокет
- Прием ICMPv4 Echo Reply и извлечение MAC-адреса отправителя
- Работа без сторонних библиотек, без исключений и без `libstdc++`
- Код написан на C под Linux

## Сборка

```
make
```
## Запуск
```
sudo ./ping_mac <интерфейс> <IPv4-адрес>
```
## Интерфейсы
eth0, enp0s3, eno1 - Ethernet\
wlan0, wlp3s0 - Wi-Fi интерфейс
