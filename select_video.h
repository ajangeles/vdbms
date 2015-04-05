void selectVideo(MYSQL *connect, char query[]){
	MYSQL_RES *res; 
	MYSQL_ROW row;
	MYSQL_FIELD *field;

	res = mysql_store_result(connect);
	int num_fields = mysql_num_fields(res);

	for(int i=0;field = mysql_fetch_field(res); i++) {
		printf("%s\t", field->name);
	}

	printf("\n");

	while ((row = mysql_fetch_row(res)))
	{
	   for(int i = 0; i < num_fields; i++)
	   {
			  if(row[i] != NULL)
			   printf("%s\t", row[i]);
		   else
				cout << "NULL" << endl;
	   }
	}

	printf("\n");

	if(res != NULL)
		mysql_free_result(res);
}