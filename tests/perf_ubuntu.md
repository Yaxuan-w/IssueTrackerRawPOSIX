## To run `perf` on Ubuntu 22.04 with kernel-6.5.0

In order to use perf on ubuntu 22.04 with newest kernel, we need to build perf from source because there’s no suitable perf version works for kernel 6.5.0. Here’s the instruction about installation:

```sh
git clone https://git.kernel.org/pub/scm/libs/libtrace/libtraceevent.git
cd libtraceevent
sudo make install
sudo apt-get install build-essential git flex bison libdw-dev libunwind-dev libssl-dev libslang2-dev \
libperl-dev llvm-dev liblzma-dev libzstd-dev libnuma-dev libbabeltrace-dev \
libcapstone-dev libpfm4-dev systemtap-sdt-dev pkg-config libstdc++-9-dev
sudo ldconfig

git clone --depth 1 https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
cd linux/tools/perf/
make NO_JEVENTS=1
sudo cp perf /usr/bin
```