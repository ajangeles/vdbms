void deleteVideo(MYSQL *connect, char query[]){
	std::string que = query;
	std::string video_name;
	std::string video_ext;
	char que2[100] = "";
	char sql2[100] = "";
	char path2[100] = "";
	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields;
		
	if (strstr(query, "videos")){
		que.replace(0, 6, "SELECT video_name, video_ext");

		for(int i=0;i<que.length();i++){
			que2[i] = que[i];
		}
	
		if(mysql_query(connect, que2) == 0){
			res = mysql_store_result(connect);
			num_fields = mysql_num_fields(res);
			row = mysql_fetch_row(res);
			video_name = row[0];
			video_ext = row[1];

			if(res != NULL)
			   mysql_free_result(res);

			std::string sql = "UPDATE videos SET is_valid = 'N' where video_name = '";
			sql.append(video_name).append("'");

			for(int i=0;i<sql.length();i++){
				sql2[i] = sql[i];
			}

			if(mysql_query(connect, sql2) == 0){
				std::string path = "C:/Users/Kevin/Documents/Video Database/Videos/";
				path.append(video_name).append(".").append(video_ext);

				for(int i=0;i<path.length();i++){
					path2[i] = path[i];
				}

				if(remove(path2)!= 0){
					perror("Error deleting file");
				}
				else{
					puts("File successfully deleted");
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
		else{
			printf(mysql_error(connect));
			printf("\n");
		}
	}
	else if (strstr(query, "videoclips")){
		que.replace(0, 6, "SELECT videoclip_name, videoclip_ext");

		for(int i=0;i<que.length();i++){
			que2[i] = que[i];
		}
	
		if(mysql_query(connect, que2) == 0){
			res = mysql_store_result(connect);
			num_fields = mysql_num_fields(res);
			row = mysql_fetch_row(res);
			video_name = row[0];
			video_ext = row[1];

			if(res != NULL)
			   mysql_free_result(res);

			std::string sql = "UPDATE videoclips SET is_valid = 'N' where videoclip_name = '";
			sql.append(video_name).append("'");

			for(int i=0;i<sql.length();i++){
				sql2[i] = sql[i];
			}

			if(mysql_query(connect, sql2) == 0){
				std::string path = "C:/Users/Kevin/Documents/Video Database/Video Clips/";
				path.append(video_name).append(".").append(video_ext);

				for(int i=0;i<path.length();i++){
					path2[i] = path[i];
				}

				if(remove(path2)!= 0){
					perror("Error deleting file");
				}
				else{
					puts("File successfully deleted");
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
		else{
			printf(mysql_error(connect));
			printf("\n");
		}
	}
}