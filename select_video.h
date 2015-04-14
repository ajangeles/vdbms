void selectVideo(MYSQL *connect, char query[]){
	MYSQL_RES *res; 
	MYSQL_ROW row;
	MYSQL_FIELD *field;

	res = mysql_store_result(connect);
	int num_fields = mysql_num_fields(res);
	int num_rows = mysql_num_rows(res);
	int videosArray[2];
	String videosStrArray[2];
	int videoclipsArray[3];
	String videoclipsStrArray[3];
	int count = 0;

	if(strstr(query, "videos")){
		if(num_rows>0){
			for(int i=0;field = mysql_fetch_field(res); i++) {
				std::string temp = field->name;
				if(temp == "video_name"){
					printf("|          %-20s|", field->name);
					videosArray[count] = i;
					videosStrArray[count] = field->name;
					count++;
				}
				else if(temp == "date_added"){
					printf("     %-15s |", field->name);
					videosArray[count] = i;
					videosStrArray[count] = field->name;
					count++;
				}
			}
			printf("\n");
			while ((row = mysql_fetch_row(res)))
			{
				count = 0;
			   for(int i = 0; i < num_fields; i++)
			   {
					  if(row[i] != NULL){
						if(videosArray[count] == i && videosStrArray[count] == "video_name"){
							printf("| %-28s |", row[i]);
							count++;
						}
						else if(videosArray[count] == i && videosStrArray[count] == "date_added"){
							printf(" %-15s |", row[i]);
							count++;
						}
					  }
				}	
			   printf("\n");
			}
			
		}
		else{
			printf("Empty set.\n");
		}	
	}
	else if(strstr(query, "videoclips")){
		if(num_rows>0){
			for(int i=0;field = mysql_fetch_field(res); i++) {
				std::string temp = field->name;
				if(temp == "videoclip_name"){
					printf("|        %-18s  |", field->name);
					videoclipsArray[count] = i;
					videoclipsStrArray[count] = field->name;
					count++;
				}
				else if(temp == "position"){
					printf("         %-17s|", field->name);
					videoclipsArray[count] = i;
					videoclipsStrArray[count] = field->name;
					count++;
				}
				else if(temp == "date_added"){
					printf("     %-15s |", field->name);
					videoclipsArray[count] = i;
					videoclipsStrArray[count] = field->name;
					count++;
				}
			}
			printf("\n");
			while ((row = mysql_fetch_row(res)))
			{
				count = 0;
			   for(int i = 0; i < num_fields; i++)
			   {
					  if(row[i] != NULL){
						if(videoclipsArray[count] == i && videoclipsStrArray[count] == "videoclip_name"){
							printf("| %-26s |", row[i]);
							count++;
						}
						else if(videoclipsArray[count] == i && videoclipsStrArray[count] == "position"){
							printf(" %-24s |", row[i]);
							count++;
						}
						else if(videoclipsArray[count] == i && videoclipsStrArray[count] == "date_added"){
							printf(" %-15s |", row[i]);
							count++;
						}
					  }
				}	
			   printf("\n");
			}

		}
		else{
			printf("Empty set.\n");
		}	
	}

	if(res != NULL)
		mysql_free_result(res);
}