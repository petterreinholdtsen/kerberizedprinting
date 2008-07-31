Credits : 
-> Alexandre Martins-Melos (SE Large Account France)
-> Michael Andrieux (SE Large Account France)



2 files : 
  1.ksmb :(sh script)  backend for CUPS which dump the spool file  /tmp 
-> You  must put it in /usr/libexec/cups/backend/
-> or : put the file in /usr/local/bin , create a symb link to it : ln -fh -s "/usr/local/bin/ksmb" "/Volumes/.../usr/libexec/cups/backend/ksmb"


2. user_kprintd (binary : A daemon to be launched by a login hook in order to be in in the
user's context which would have access to the in-memory ticket cache. 
-> You can put it where ever you want for example in /usr/local/bin/

The process :
-> Add like usual a printer using the ksmb printer : ksmb://serveur/imprimante WITHOUT name and password in the URI (kerberos !!)
-> Just works !

