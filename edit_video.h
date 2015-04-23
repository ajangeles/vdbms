void editVideo(MYSQL *connect, char query[]){
	if(strstr(query, "videos")){
		if(strstr(query, "video_id") || strstr(query, "video_ext") || strstr(query, "video_path") || strstr(query, "date_added") || strstr(query, "is_valid") || strstr(query, "position") || strstr(query, "detect_count")){
			printf("Invalid query. You can only edit the name of a video/videoclip.\n");
		}
		else if(strstr(query, "video_name")){
			if(mysql_query(connect, query) == 0){
				printf("Query OK.\n");
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
	else if(strstr(query, "videoclips")){
		if(strstr(query, "videoclip_id") || strstr(query, "video_id") || strstr(query, "date_added") || strstr(query, "position") || strstr(query, "is_valid")){
			printf("Invalid query. You can only edit the name of a video/videoclip.\n");
		}
		else if(strstr(query, "videoclip_name")){
			if(mysql_query(connect, query) == 0){
				printf("Query OK.\n");
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
}