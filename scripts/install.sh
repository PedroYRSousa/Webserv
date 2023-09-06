rm -rf googletest;
sudo apt-get update -y;
sudo apt-get upgrade -y;
sudo apt-get install curl g++ gcc cmake make cppcheck apt-transport-https -y;
git clone https://github.com/google/googletest.git;
cd googletest && mkdir -p build && cd build && cmake .. && sudo make && sudo make install && cd .. && cd .. && rm -rf googletest;
