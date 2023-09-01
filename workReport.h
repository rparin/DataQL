#ifndef WORKREPORT_H
#define WORKREPORT_H
/*
Features:
		-Not Implemented:
			-Delete Where

		-Implemented:
		-Program uses Maps and Multimaps based on B+Trees

		-Table Class:
			-Constructor takes in name of file (Opens existing file)
			-Constructor takes in name of file, and fields (Creates file)
			-Insert into, Select where, Select, ostream operator

		Parser Class:
			-makeTable()
			-makeparsetree()

			-<SELECT> : {  select <* | FIELD_NAME> [, ,FIELD_NAME>...]
					from <TABLE_NAME>

			-<INSERT> : { insert <INTO> <TABLE_NAME> 
						values <VALUE> [, <VALUE>...]}

			-<CREATE | MAKE> : 
				{  <create | make> table <TABLE_NAME> 
				fields <FIELD_NAME> [, <FIELD_NAME>...]  }

			-<VALUE>  : A string of alphanumeric 
			characters, or a string of alphanumeric
 			characters and spaces enclosed by double quotation marks:
 			"Jean Luise", Finch, 1923

			-<LOAD> <table name>

						-Parser for Select Where
				where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>
						[<LOGICAL_OPERATOR>
							<FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...]
			-<RELATIONAL OPERATOR> : [ = ]

		-Partly implemented: 


Bugs: None LE LT and GE are sometimes off by 1

Reflections:None
*/

#endif // WORKREPORT_H