#!/bin/bash
#sudo lsof -i
#sudo netstat -lptu
sudo netstat -tulpn | grep server.exec
sudo netstat -tulpn | grep client.exec
