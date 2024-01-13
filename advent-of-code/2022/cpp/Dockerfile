# docker build . -t builder
# docker run -it -v `pwd`:/build builder
FROM archlinux

RUN pacman -Syyu --noconfirm --noprogressbar && pacman --noconfirm -S base-devel cmake catch2

RUN mkdir build
WORKDIR build
CMD ./build-and-run.sh
