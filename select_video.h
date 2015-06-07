/*
	NOTE: The user's input is an select statement. 
		  For videos table, the user can only view the video_id, video_name, and date_added columns.
		  For videoclips table, the user can only view the videoclip_id, videoclip_name, position, and date_added columns.
		  
	Basic Syntax:
		videos table: Select * from videos;
		videoclips table: Select * from videoclips;
		
		NOTE: Other select statement that is valid is accepted. 
		
	This function:
	1. Checks whether the user wants to view the data in the videos or videoclips table.
	2. If the table is not empty, the program will print first the column names followed by the generated value of each row for each column.
*/

void selectVideo(MYSQL *connect, char query[]){
	MYSQL_RES *res; 
	MYSQL_ROW row;
	MYSQL_FIELD *field;

	res = mysql_store_result(connect);
	int num_fields = mysql_num_fields(res);
	int num_rows = mysql_num_rows(res);
	int videosArray[3]; //store the column names of videos table
	String videosStrArray[3]; //this is where the value for each column is stored. index 0 = video_id, index 1 = video_name, index 2 = date_added
	int videoclipsArray[5]; //store the column names of videoclips table
	String videoclipsStrArray[5]; //this is where the value for each column is stored. index 0 = videoclip_id, index 1 = videoclip_name, index 2 = position, index 3 = date_added
	int count = 0;

	if(strstr(query, "videos")){ //if the user wants to view the data in the videos table
		if(num_rows>0){ //the videos table is not empty
			for(int i=0;field = mysql_fetch_field(res); i++) {
				//print the column names in the console
				std::string temp = field->name;
				if(temp == "video_id"){
					printf("| %-5s ", field->name);
					videosArray[count] = i;
					videosStrArray[count] = field->name;
					count++;
				}
				else if(temp == "video_name"){
					printf("|                 %-27s|", field->name);
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
						 //print the column values of each row in the console
						if(videosArray[count] == i && videosStrArray[count] == "video_id"){
							printf("|    %-5s ", row[i]);
							count++;
						}
						else if(videosArray[count] == i && videosStrArray[count] == "video_name"){
							printf("| %-42s |", row[i]);
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
			
		} //the videos table is empty.
		else{
			printf("Empty set.\n");
		}	
	}
	else if(strstr(query, "videoclips")){ //if the user wants to view the data in the videoclips table
		if(num_rows>0){ //the videoclips table is not empty
			for(int i=0;field = mysql_fetch_field(res); i++) {
				//print the column names in the console
				std::string temp = field->name;
				if(temp == "videoclip_id"){
					printf("| %-5s ", field->name);
					videoclipsArray[count] = i;
					videoclipsStrArray[count] = field->name;
					count++;
				}
				else if(temp == "videoclip_name"){
					printf("|        %-17s     |", field->name);
					videoclipsArray[count] = i;
					videoclipsStrArray[count] = field->name;
					count++;
				}
				else if(temp == "video_id"){
					printf(" %-5s |", field->name);
					videoclipsArray[count] = i;
					videoclipsStrArray[count] = field->name;
					count++;
				}
				else if(temp == "position"){
					printf("              %-22s|", field->name);
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
						//print the column values of each row in the console
						if(videoclipsArray[count] == i && videoclipsStrArray[count] == "videoclip_id"){
							printf("|      %-7s ", row[i]);
							count++;
						}
						else if(videoclipsArray[count] == i && videoclipsStrArray[count] == "videoclip_name"){
							printf("| %-28s |", row[i]);
							count++;
						}
						else if(videoclipsArray[count] == i && videoclipsStrArray[count] == "video_id"){
							printf("    %-5s |", row[i]);
							count++;
						}
						else if(videoclipsArray[count] == i && videoclipsStrArray[count] == "position"){
							printf("  %-33s |", row[i]);
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
		else{ //the videoclips table is empty
			printf("Empty set.\n");
		}	
	}
	else{
		printf("Invalid query.\n");
	}

	if(res != NULL)
		mysql_free_result(res);
}