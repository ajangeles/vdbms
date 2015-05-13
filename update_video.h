/* NOTE: The user's input is an update statement. 
		 The user can only update the name of a video/videoclip.
		  
	Basic Syntax:
		videos table: UPDATE videos SET video_name = '<new_video_name>' where video_name = '<old_video_name>';
			Ex. UPDATE videos SET video_name = 'videoOne' where video_name = 'video1';
		videoclips table: UPDATE videoclips SET videoclip_name = '<new_videoclip_name>' where videoclip_name = '<old_videoclip_name>';
			Ex. UPDATE videoclips SET videoclip_name = 'videoclipOne' where videoclip_name = 'videoclip1';

		NOTE: These are the only accepted query for UPDATE statement.

	This function:
	1. Check whether the user will edit data in the video or videoclips table.
	2. Reject the user's query if other column/s aside from video_name (if user wants to edit data in videos table) or videoclip_name (if user wants to edit data in videoclips table)
	3. If only the video_name or videoclip_name column is found in the user's query, execute query.

	Possible Errors:
		- video_name/videoclip_name already exist.
		- User generated more than 1 row (use of LIKE in query).
*/

void updateVideo(MYSQL *connect, char query[]){
	if(strstr(query, "videos")){ //check if the user wants to edit data in the videos table
		if(strstr(query, "video_id") || strstr(query, "video_ext") || strstr(query, "video_path") || strstr(query, "date_added") || strstr(query, "is_valid") || strstr(query, "position") || strstr(query, "detect_count")){ //User can only edit video_name. If other column is found, the user's query is rejected.
			printf("Invalid query. You can only edit the name of a video/videoclip.\n");
		}
		else if(strstr(query, "video_name")){ //if only the video_name column is found in the user's query
			if(mysql_query(connect, query) == 0){ //execute query
				printf("Query OK.\n");
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
	else if(strstr(query, "videoclips")){ //check if the user wants to edit data in the videos table
		if(strstr(query, "videoclip_id") || strstr(query, "video_id") || strstr(query, "date_added") || strstr(query, "position") || strstr(query, "is_valid")){ //User can only edit videoclip_name. If other column is found, the user's query is rejected.
			printf("Invalid query. You can only edit the name of a video/videoclip.\n");
		}
		else if(strstr(query, "videoclip_name")){ //if only the videoclip_name column is found in the user's query
			if(mysql_query(connect, query) == 0){ //execute query
				printf("Query OK.\n");
			}
			else{
				printf(mysql_error(connect));
				printf("\n");
			}
		}
	}
}