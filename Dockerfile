FROM ubuntu

RUN apt-get update
RUN apt-get  install dos2unix
RUN apt -y install build-essential

WORKDIR /tests
COPY . .
RUN chmod +x all.sh
RUN ls -al 
CMD [ "/tests/all.sh"] 

