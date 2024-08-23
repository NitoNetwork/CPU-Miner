# Nito CPU-miner
A Nito optimized cpu miner.

# Build for Linux
```bash
sudo apt-get install build-essential binutils automake libssl-dev libcurl4-openssl-dev lib32z1-dev libjansson-dev libgmp-dev git

git clone https://github.com/NitoNetwork/CPU-Miner/

cd CPU-Miner

chmod +x *.sh

./build.sh
```


***Run node && cpuminer on the same PC!***

# Linux Solo - Mainnet - sha256d
./cpuminer -a sha256d -o http://127.0.0.1:8825/ --userpass=user:pass --no-getwork --no-stratum --coinbase-addr=nito1qmf7tvcfmq942ry2j0ysyxg69dd666fn5gxaprl -D

# Linux Solo - Testnet - sha256d
./cpuminer -a sha256d -o http://127.0.0.1:8845/ --userpass=user:pass --no-getwork --no-stratum --coinbase-addr=tn1qu4ftg7z3e2nph5nd2kzauts53tzus5t5kc7qfv -D


# Help
./cpuminer --help

# Missing libraries?
libgmp3-dev zlib1g-dev

# Mainnet config ( nito.conf )
```bash
maxconnections=300
listen=1
server=1

rpcuser=user
rpcpassword=pass
rpcallowip=0.0.0.0/0
rpcport=8825
```
# Testnet config ( nito.conf )
```bash
maxconnections=300
testnet=1
listen=1
server=1

[test]
rpcuser=user
rpcpassword=pass
rpcallowip=0.0.0.0/0
rpcport=8845
```
