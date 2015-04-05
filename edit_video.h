void editVideo(MYSQL *connect, char query[]){
	int count = 0;

	for(int i=0; i<strlen(query); i++){
		if(query[i] == ' '){
			 count++;
		 }
	}

	count++;

	if(strstr(query, "videos")){
		if(strstr(query, "video_id") || strstr(query, "video_ext") || strstr(query, "video_path") || strstr(query, "date_added") || strstr(query, "is_valid")){
			printf("The UPDATE statement template is: UPDATE <videos/videoclips> SET <video_name/videoclip_name> = <new_name> WHERE <video_name/videoclip_name = <old_name>\n");
		}
		else if(strstr(query, "video_name") && count == 10){
			if(mysql_query(connect, query) == 0){
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
	else if(strstr(query, "videoclips")){
		if(strstr(query, "videoclip_id") || strstr(query, "videoclip_ext") || strstr(query, "videoclip_path") || strstr(query, "video_id") || strstr(query, "date_added") || strstr(query, "is_valid")){
			printf("The UPDATE statement template is: UPDATE <videos/videoclips> SET <video_name/videoclip_name> = <new_name> WHERE <video_name/videoclip_name = <old_name>\n");
		}
		else if(strstr(query, "videoclip_name") && count == 10){
			if(mysql_query(connect, query) == 0){
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
}