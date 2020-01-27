FROM ubuntu:18.04 as builder

USER root

# Locale
ENV LC_ALL C
ENV LC_ALL C.UTF-8
ENV LANG C.UTF-8

# ALL tool versions used by opt-build.sh
ENV VER_HTSLIB="1.9"
ENV VER_FQTOOLS="v2.3"

RUN apt-get -yq update
RUN apt-get install -yq \
autoconf \
gcc \
make \
curl \
zlib1g-dev \
libcurl4-openssl-dev \
libssl-dev \
libbz2-dev \
liblzma-dev \
libhts-dev \
python3-dev

ENV OPT /opt/wsi-t113
ENV PATH $OPT/bin:$PATH
ENV LD_LIBRARY_PATH $OPT/lib

ADD build/opt-build.sh build/
RUN bash build/opt-build.sh $OPT

FROM ubuntu:18.04

LABEL maintainer="ad33@sanger.ac.uk" \
      version="v2.3" \
      description="fqtools Docker container"

MAINTAINER  Alastair Droop <ad33@sanger.ac.uk>

RUN apt-get -yq update

RUN apt-get install -yq \
zlib1g \
libcurl4 \
libssl1.1 \
libbz2-1.0 \
liblzma5 \
libhts2 \
bc \
python3

ENV OPT /opt/wsi-t113
ENV PATH $OPT/bin:$OPT/python3/bin:$OPT/fqtools/bin:$PATH
ENV LD_LIBRARY_PATH $OPT/lib:$OPT/fqtools/htslib/include/htslib:$OPT/fqtools/htslib/lib
ENV PYTHONPATH $OPT/python3:$OPT/python3/lib/python3.6/site-packages
ENV LC_ALL C
ENV LC_ALL C.UTF-8
ENV LANG C.UTF-8
ENV DISPLAY=:0

RUN mkdir -p $OPT
COPY --from=builder $OPT $OPT

#Create some usefull symlinks
RUN cd /usr/local/bin && \
    ln -s /usr/bin/python3 python

## USER CONFIGURATION
RUN adduser --disabled-password --gecos '' ubuntu && chsh -s /bin/bash && mkdir -p /home/ubuntu

USER ubuntu
WORKDIR /home/ubuntu

CMD ["/bin/bash"]
