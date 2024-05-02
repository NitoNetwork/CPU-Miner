# cpuminer
A Nito optimized cpu miner.

# Build for Linux
```bash
sudo apt-get install build-essential binutils automake libssl-dev libcurl4-openssl-dev lib32z1-dev libjansson-dev libgmp-dev git

git clone https://github.com/NitoCoin/CPU-Miner/

cd cpuminer

chmod +x *.sh

./build.sh
```


***Run node && cpuminer on the same PC!***

# Linux Solo - sha256d
./cpuminer -a sha256d -o http://127.0.0.1:12250/ --userpass=user:pass --no-getwork --no-stratum --coinbase-addr=tn1qqch4k04lp5wkpy7v0a23h3wy0dgyyh0jjdmt2q -D


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
rpcport=12250
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
rpcport=12250
```