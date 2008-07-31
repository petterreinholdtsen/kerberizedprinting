#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <sys/types.h>
#include <dirent.h>

#define kSTRING_BUFFER_SIZE 1024
#define kDEFAULT_CLIENT_SPOOL_DIR "/var/spool/samba/clientspool"

char g_spool_dir[kSTRING_BUFFER_SIZE];

void sighup_handler(int n) 
{
	struct passwd *current_user_info = getpwuid(getuid());
	char user_spool_dir_path[kSTRING_BUFFER_SIZE];
	char printserver_dir_path[kSTRING_BUFFER_SIZE];
	char queue_dir_path[kSTRING_BUFFER_SIZE];
	char file_dir_path[kSTRING_BUFFER_SIZE];
	char file_path[kSTRING_BUFFER_SIZE];
	char print_command[kSTRING_BUFFER_SIZE];
	DIR *printserver_dir;
	DIR *queue_dir;
	DIR *file_dir;
	struct dirent *printserver_ptr;
	struct dirent *queue_ptr;
	struct dirent *file_ptr;
	int rc;
	
	if(current_user_info == NULL) {
		fprintf(stderr, "current_user_info=NULL\n");
		return;
	}
	
	sprintf(user_spool_dir_path, "%s/%s", g_spool_dir, current_user_info->pw_name);
	
	//fprintf(stderr, "user_spool_dir_path=%s\n", user_spool_dir_path);
	
	printserver_dir = opendir(user_spool_dir_path);
	if(printserver_dir == NULL) {
		fprintf(stderr, "printserver_dir=NULL\n");
		return;
	}
			
	while(printserver_ptr = readdir(printserver_dir))
		if(printserver_ptr->d_name[0] != '.') {
			sprintf(queue_dir_path, "%s/%s", user_spool_dir_path, printserver_ptr->d_name);
			queue_dir = opendir(queue_dir_path);
			if(queue_dir != NULL) {
				while(queue_ptr = readdir(queue_dir))
					if(queue_ptr->d_name[0] != '.') {
						sprintf(file_dir_path, "%s/%s", queue_dir_path, queue_ptr->d_name);
						file_dir = opendir(file_dir_path);
						if(file_dir != NULL) {
							while(file_ptr = readdir(file_dir))
								if((file_ptr->d_name[0] != '.') && (strncmp((const char*)file_ptr->d_name, "print.", 6) == 0)) {
									sprintf(file_path, "%s/%s", file_dir_path, file_ptr->d_name);
									sprintf(print_command, "/usr/bin/smbclient -k -c \"print %s\" //%s/%s\n", file_path, printserver_ptr->d_name, queue_ptr->d_name);
									//fprintf(stderr, "print_command=%s\n", print_command);
									rc = system(print_command);
									if((rc >= 0) && (rc != 127)) {
										rc = remove(file_path);
										if(rc != 0)
											fprintf(stderr, "cannot remove file %s=%d\n", file_path, rc);
									}
									else
										fprintf(stderr, "print_command error=%d\n", rc);
								}
							closedir(file_dir);
						}
					}
				closedir(queue_dir);
			}
		}

	closedir(printserver_dir);
}

int main(int argc, const char **argv) 
{

#ifdef __DAEMON__
	daemon(0, 0);  
#endif

	if((argc > 1) && (strlen(argv[1]) > 1) && (access(argv[1], F_OK) == 0))
		sprintf(g_spool_dir, "%s", argv[1]);
	else
		sprintf(g_spool_dir, "%s", kDEFAULT_CLIENT_SPOOL_DIR);
  
	fprintf(stderr, "user_kprintd=%d, user=%d, g_spool_dir=%s\n", getpid(), getuid(), g_spool_dir);
  
	signal(SIGUSR1, sighup_handler);

	while(1) {
                sleep(500);
	}

	return 0;
}
