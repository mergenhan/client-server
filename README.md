# client-server
homework for cloud computing

Projede bir server ve birden fazla istemci arasında grup olarak iletişim kurulabilmesi ve geliştirilmesi hedeflenmektedir. Bunu başarmak için sohbet odası uygulaması geliştirdim. Bu program 20 kişiye kadar istemci kabul etmektedir. 20 kadara istemci server.c dosyası çalıştırıldığı takdirde birbirleriyle ortak olarak iletişim kurabilmektedir.

Sunucu ve istemciden oluşur. Sunucu, istemcilerden alınan ve gönderilen her işlemi yönetir. Sunucu, kullanıcıların şunları yapmasına izin verebilir: 
1. Kullanıcı adı oluşturulur. 
2. Belirli bir müşteriye doğrudan mesaj gönderilir. 
3. Grupla sohbet edilir. 
4. Uygulamadan bir kullanıcının çıkması durumunda diğer istemcilere de bildirim gönderilir. 

Ödev nasıl derlenir ve çalıştırılır? 
Ubuntu 18.04 işletim sisteminde test edilmiştir. Hem sunucu hem de istemci gcc ile derlenebilir. 
gcc client.c -o client 
gcc server.c -o server -pthread 

Derleme işleminden sonra: 
./server (Bu, sunucuyu çalıştıracaktır) 
./client <IP adresi> (Bu, istemciyi çalıştıracaktır) 

Ödevde bulunan özellikler nasıl kullanılır? 
Server.c dosyası çalıştıktan sonra istemciler ile aşağıdaki işlevleri kullanabilirsiniz: 
Takma adınızı değiştirmek için #newname. 
Programdan çıkmak için #exit. 
Doğrudan mesaj göndermek için #dm

In this project, it is aimed to develop and application for client-server as a client-group. To achieve it, we developed client-server like chat room application. This program has a capability of 20 people to chatting instantly.
It consist of server and client. Server handles every process that is received and sent from clients. Server can allow users to do followings:

	1.	Change their name.
	2.	Send direct messages to specific client.
	3.	Chatting with group.
	4.	Client can exit from group.
	5.	If one user exit from the chatting application, we are announced other users.


How to compile and run?

It is tested under Linux. Both server and client can be compiled with gcc. Followings can be enough for compiling it:

gcc client.c -o client
gcc server.c -o server -pthread
After compilation:
./server (This will run server)
./client <IP address> (This will run client)
How to use functions?
After you are entering chat room, you can use following function with the character:
	•	#newname for changing your nickname.
	•	#exit to quit program.
	•	#dm for sending direct message.
