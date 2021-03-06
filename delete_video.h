void deleteVideo(MYSQL *connect, char query[]){
	std::string que = query;
	std::string video_name;
	char que2[100] = "";
	char sql2[100] = "";
	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_rows;
	char ans;

	printf("Are you sure?[Y/N]: ");
	ans = getch();
	printf("%c", ans);
	if(ans == 'y' || ans == 'Y'){
		printf("\n");
		if (strstr(query, "videos")){
			que.replace(0, 6, "SELECT video_name");

			for(int i=0;i<que.length();i++){
				que2[i] = que[i];
			}
	
			if(mysql_query(connect, que2) == 0){
				res = mysql_store_result(connect);
				num_rows = mysql_num_rows(res);

				if(num_rows == 0){
					printf("Empty set.\n");	
				}
				else{
					for(int i=0;i<num_rows;i++){
						row = mysql_fetch_row(res);
						video_name = row[0];

						std::string sql = "UPDATE videos SET is_valid = 'N' where video_name = '";
						sql.append(video_name).append("'");

						for(int i=0;i<sql.length();i++){
							sql2[i] = sql[i];
						}

						if(mysql_query(connect, sql2) == 0){
							std::string path = "C:/Video Database/Videos/";
							path.append(video_name).append(".").append("avi");
							char path2[100] = "";

							for(int i=0;i<path.length();i++){
								path2[i] = path[i];
							}

							if(remove(path2)!= 0){
								perror("Error deleting file");
							}
							else{
								cout << video_name << ".avi successfully deleted.\n";
								if(mysql_query(connect, query) == 0){
								}
								else{
									printf(mysql_error(connect));
									printf("\n");
								}
							}
						}
						else{
							printf(mysql_error(connect));
							printf("\n");
						}
					}
				}
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
		else if (strstr(query, "videoclips")){
			que.replace(0, 6, "SELECT videoclip_name");

			for(int i=0;i<que.length();i++){
				que2[i] = que[i];
			}
	
			if(mysql_query(connect, que2) == 0){
				res = mysql_store_result(connect);
				num_rows = mysql_num_rows(res);

				if(num_rows == 0){
					printf("Empty set.\n");	
				}
				else{
					for(int i=0;i<num_rows;i++){
						row = mysql_fetch_row(res);
						video_name = row[0];

						std::string sql = "UPDATE videoclips SET is_valid = 'N' where videoclip_name = '";
						sql.append(video_name).append("'");

						for(int i=0;i<sql.length();i++){
							sql2[i] = sql[i];
						}

						if(mysql_query(connect, sql2) == 0){
							std::string path = "C:/Video Database/Video Clips/";
							path.append(video_name).append(".").append("avi");
							char path2[100] = "";

							for(int i=0;i<path.length();i++){
								path2[i] = path[i];
							}

							if(remove(path2)!= 0){
								perror("Error deleting file");
							}
							else{
								cout << video_name << ".avi " << "successfully deleted.\n";
								if(mysql_query(connect, query) == 0){
								}
								else{
									printf(mysql_error(connect));
									printf("\n");
								}
							}
						}
						else{
							printf(mysql_error(connect));
							printf("\n");
						}
					}
				}
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
	else{
		printf("\n");
	}
}