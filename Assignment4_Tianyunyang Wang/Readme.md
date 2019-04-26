- Do the following 

- Compile

rpcgen rpc.x
cc server.c rpc_svc.c -o server -lnsl
cc client.c rpc_clnt.c -o client -lnsl

> if run on mac os, eliminate the `-lnsl`

- Run

./server
./client <hostname> 


- Enter the alphanumeric string with secret word C00l

123C00L


- Check secrets.out on server

5: 123C00L
