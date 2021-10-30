#include <iostream>
#include <queue>
#include <stdio.h>
#include <vector>
#include <fstream>

using namespace std;

class gridcell
{
public:
	int x = -1;
	int y = -1;
	int pathcost=0;
	int cost = 1;
	char predecessor = '0';
	int reached = 0;

public:
	int get_pathcost() const
	{
		return pathcost;
	}

};

class compare_pathcost
{
public:
	int operator() (const gridcell& c1, const gridcell& c2)
	{
		return c1.get_pathcost() > c2.get_pathcost();
	}

};


/* Very important : Confusing ------>  vector[y][x] is the cell at row y and column x in the grid.
Its the cell at x and y coordinates. */

vector<vector<gridcell>> create_grid()
{
	/*Creates grid 2D vector by reading input grid file and returns grid*/
	ifstream grid_input;
	grid_input.open("bench2.grid");
	
	int m, n = 0;
	int garbage;
	
	grid_input >> n;  //X-size or number of columns
	grid_input >> m;  //Y-size or number of rows
	vector <vector<gridcell>> grid (m, vector<gridcell>(n)); //Creating a 2D vector (named grid) of gridcell objects (m rows and n columns)

	//Waste out the bend penalty
	grid_input >> garbage; grid_input >> garbage;

	//Read all the costs of all cells from the grid file. Fill row by row i.e, y-coordinate by y-coordinate
	for (int i = 0; i < m; i++)     //i row number or y coordinate
	{
		for (int j = 0; j < n; j++)  //j holds the column number or x coordinate
		{
			grid_input >> grid[i][j].cost;   //Set costs of grid cells
			grid[i][j].x = j;			 //Set x coordinate and y coordinate of gridcells. //Note that index and actual coordinates of gridcells differ by 1.
			grid[i][j].y = i;
		}
	}

	grid_input.close();

	return grid;
	
}


gridcell read_source_and_destination(vector<vector<gridcell>> &grid, priority_queue <gridcell, vector<gridcell>, compare_pathcost> &wavefront)
{
	/*Reads a line on netlist.txt. Inserts sourcecell into wavefront and returns destination cell*/
	ifstream netlist;
	netlist.open("sample_bench.nl");

	//Waste number of nets and net id
	int garbage;
	netlist >> garbage;
	netlist >> garbage;
	netlist >> garbage; //Waste layer number

	gridcell sourcell, destinationcell;
	netlist >> sourcell.x;
	netlist >> sourcell.y;

	netlist >> garbage; //Waste layer number of destination cell
	netlist >> destinationcell.x;
	netlist >> destinationcell.y;

	//Push source cell in wavefront
	wavefront.push(sourcell);
	netlist.close();
	return destinationcell;

}


void wavefront_expansion(priority_queue <gridcell, vector<gridcell>, compare_pathcost> &wavefront, vector<vector<gridcell>> &grid, gridcell &destinationcell)
{
	/* This function expands out of the source cell and reaches the destination cell. The gridcells on the actual
	   grid 2D-Vector are modified accordingly as the expansion occurs.*/


	//Set cell_to_be_expanded initially as the source cell poped from wavefront
	gridcell cell_to_be_expanded, source_cell;
	source_cell = wavefront.top();
	wavefront.pop();
	cell_to_be_expanded = source_cell;

	cell_to_be_expanded.reached = 1;
	cell_to_be_expanded.pathcost = cell_to_be_expanded.cost;  //Set pathcost of source_cell as its cost

	//Now, copy the parameter values of cell_to_be_expanded to the actual source-cell on source-cell coordinates on the grid vector.
	grid[cell_to_be_expanded.x][cell_to_be_expanded.y] = cell_to_be_expanded;

	int number_of_rows_in_grid = grid.size();
	int number_of_columns_in_grid = grid[0].size();

	

	
	/*This loop takes the cell to be expanded and reaches all its neighbours on the grid vector and 
	changes their parameters on grid vector accordingly*/
	//While cell to be expanded != destination_cell, execute the loop.
	while ( !(cell_to_be_expanded.x == destinationcell.x && cell_to_be_expanded.y == destinationcell.y) ) 
	{
		int x = cell_to_be_expanded.x;
		int y = cell_to_be_expanded.y;  //x and y hold the coordinates of the cell to be expanded
		int to_reach_x;
		int to_reach_y;

		gridcell cell_to_reach; //Temporary object
		

		//Expand to Upper Cell
		//--------------------------------------------------------------------------------------------------------------------------
		to_reach_x = x;
		to_reach_y = y + 1;

		
		//Initialise cell_to_reach object appropriately , given to_reach_x and y lie within grid coordinates
		if (((0 <= to_reach_x) && (to_reach_x < number_of_columns_in_grid)) && ((0 <= to_reach_y) && (to_reach_y < number_of_rows_in_grid)))
			cell_to_reach = grid[to_reach_y][to_reach_x]; //Note that indexes of a cell in <grid vector> is 1 less than the actual x and y coordinates

		//If cell we want to reach is within the grid and is not blocked and is not already reached, push it onto wavefront with appropriate parameters
		if ((((0 <= to_reach_x) && (to_reach_x < number_of_columns_in_grid)) && ((0 <= to_reach_y) && (to_reach_y < number_of_rows_in_grid))) && (cell_to_reach.cost != -1) && (cell_to_reach.reached!=1))
		{
			//Calculate pathcost of cell to reach and assign it that pathcost.
			cell_to_reach.pathcost = cell_to_be_expanded.pathcost + cell_to_reach.cost; 
			cell_to_reach.reached = 1;
			cell_to_reach.predecessor = 'S';  //Since it was reached from south
			grid[to_reach_y][to_reach_x] = cell_to_reach;   //Copy cell_to_reach to the gridcell above cell_to_be_expanded on grid
			wavefront.push(cell_to_reach);   //Push the reached gridcell into the wavefront
		}
		
		
		//Expand to Lower Cell
		//--------------------------------------------------------------------------------------------------------------------------

		to_reach_x = x;
		to_reach_y = y - 1;

		//Initialise cell_to_reach object appropriately , given to_reach_x and y lie within grid coordinates
		if (((0 <= to_reach_x) && (to_reach_x < number_of_columns_in_grid)) && ((0 <= to_reach_y) && (to_reach_y < number_of_rows_in_grid)))
			cell_to_reach = grid[to_reach_y][to_reach_x]; //Note that indexes of a cell in <grid vector> is 1 less than the actual x and y coordinates
		
	    //If cell we want to reach is within the grid and is not blocked and is not already reached, push it onto wavefront with appropriate parameters
		if ((((0 <= to_reach_x) && (to_reach_x < number_of_columns_in_grid)) && ((0 <= to_reach_y) && (to_reach_y < number_of_rows_in_grid))) && (cell_to_reach.cost != -1) && (cell_to_reach.reached != 1))
		{
			//Calculate pathcost of cell to reach and assign it that pathcost.
			cell_to_reach.pathcost = cell_to_be_expanded.pathcost + cell_to_reach.cost;
			cell_to_reach.reached = 1;
			cell_to_reach.predecessor = 'N';  //Since it was reached from North
			grid[to_reach_y][to_reach_x] = cell_to_reach;  //Copy values of cell_to_reach on the actual cell to reach on grid
			wavefront.push(cell_to_reach);   //Push the reached gridcell into the wavefront
		}

		
		//Expand to Right Cell
		//--------------------------------------------------------------------------------------------------------------------------
		to_reach_x = x+1;
		to_reach_y = y;


		//Initialise cell_to_reach object appropriately , given to_reach_x and y lie within grid coordinates
		if (((0 <= to_reach_x) && (to_reach_x < number_of_columns_in_grid)) && ((0 <= to_reach_y) && (to_reach_y < number_of_rows_in_grid)))
			cell_to_reach = grid[to_reach_y][to_reach_x]; //Note that indexes of a cell in <grid vector> is 1 less than the actual x and y coordinates


		//If cell we want to reach is within the grid and is not blocked and is not already reached, push it onto wavefront with appropriate parameters
		if ((((0 <= to_reach_x) && (to_reach_x < number_of_columns_in_grid)) && ((0 <= to_reach_y) && (to_reach_y < number_of_rows_in_grid))) && (cell_to_reach.cost != -1) && (cell_to_reach.reached != 1))
		{
			cell_to_reach = grid[to_reach_y][to_reach_x]; //Note that indexes of a cell in grid vector is 1 less than the actual x and y coordinates
			//Calculate pathcost of cell to reach and assign it that pathcost.
			cell_to_reach.pathcost = cell_to_be_expanded.pathcost + cell_to_reach.cost;
			cell_to_reach.reached = 1;
			cell_to_reach.predecessor = 'W';  //Since it was reached from West
			grid[to_reach_y][to_reach_x] = cell_to_reach;  //Copy values of cell_to_reach on the actual cell to reach on grid
			wavefront.push(cell_to_reach);   //Push the reached gridcell into the wavefront
		}

		
		//Expand to Left Cell
		//--------------------------------------------------------------------------------------------------------------------------
		to_reach_x = x - 1;
		to_reach_y = y;
		

		//Initialise cell_to_reach object appropriately , given to_reach_x and y lie within grid coordinates
		if (((0 <= to_reach_x) && (to_reach_x < number_of_columns_in_grid)) && ((0 <= to_reach_y) && (to_reach_y < number_of_rows_in_grid)))
			cell_to_reach = grid[to_reach_y][to_reach_x]; //Note that indexes of a cell in <grid vector> is 1 less than the actual x and y coordinates


		//If cell we want to reach is within the grid and is not blocked and is not already reached, push it onto wavefront with appropriate parameters
		if ((((0 <= to_reach_x) && (to_reach_x < number_of_columns_in_grid)) && ((0 <= to_reach_y) && (to_reach_y < number_of_rows_in_grid))) && (cell_to_reach.cost != -1) && (cell_to_reach.reached != 1))
		{
			gridcell cell_to_reach = grid[to_reach_y][to_reach_x]; //Note that indexes of a cell in grid vector is 1 less than the actual x and y coordinates
			//Calculate pathcost of cell to reach and assign it that pathcost.
			cell_to_reach.pathcost = cell_to_be_expanded.pathcost + cell_to_reach.cost;
			cell_to_reach.reached = 1;
			cell_to_reach.predecessor = 'E';  //Since it was reached from East
			grid[to_reach_y][to_reach_x] = cell_to_reach;  //Copy values of cell_to_reach on the actual cell to reach on grid
			wavefront.push(cell_to_reach);   //Push the reached gridcell into the wavefront
		}

		/* Once all the neighbours of cell_to_be_expanded are reached and pushed into wavefront, pop out the new gridcell that
		is on top of the wavefront now and update cell_to_be_expanded*/
		//if (wavefront.empty())	break;
		cell_to_be_expanded = wavefront.top();
		wavefront.pop();

	}

	/*The cell to be expanded is now equal to the destination cell object. All its parameters were already set appropriately when
	it was pushed onto wavefront previously.*/

	//Now, copy the values of cell_to_be_expanded object to the actual destination-cell on destination-cell position on the grid vector
	grid[destinationcell.y][destinationcell.x] = cell_to_be_expanded;

	//-----Set predecessor of source cell to 'X'. So that it can be identified during backtrace. 
	grid[source_cell.y][source_cell.x].predecessor = 'X';  
}


void backtrace(vector<vector<gridcell>>& grid, gridcell& destinationcell)
{
	/* Traces the created optimal path back from destination cell to source cell and prints the x and y coordinates
	   of each cell along the path */

	gridcell cell = grid[destinationcell.y][destinationcell.x]; //cell holds the destination cell object in the grid
	
	ofstream solution;
	solution.open("solution.txt");

	solution << 1 << endl; //First line in solution is number of nets
	solution << 1 << endl; //Second Line is the net number whose path we are mentioning

	//While cell != source cell... Only source cell will have predecessor as 'X' in the path we trace back from destination cell
	while (cell.predecessor != 'X')
	{
		//Write layer number followed by x and y coordinates of the present cell 
		solution << 1 << " " << cell.x << " " << cell.y << endl;   

		/*Choose new value of cell as its current predecessor------------------------------------------------------------------*/
		//If cell has been reached from north, set new value of cell as the gridcell on the grid->one position upper to present cell
		if (cell.predecessor == 'N')
			cell = grid[cell.y + 1][cell.x];  
			//(cell.y-1) is index of present cell on grid. (Cell.y-1)+1 is the index of cell one psoition upper on the grid.
	
		//If cell has been reached from south, set new value of cell as the gridcell on the grid, one position lower to present cell
		else if (cell.predecessor == 'S')
			cell = grid[cell.y - 1][cell.x];  
			//(cell.y-1) is index of present cell on grid. (Cell.y-1)-1 is the index of cell one psoition lower on the grid.
		
		//If cell has been reached from East, set new value of cell as the gridcell on the grid, one position right to present cell
		else if (cell.predecessor == 'E')
			cell = grid[cell.y][cell.x + 1];
			//(cell.x-1) is index of present cell on grid. (Cell.x-1)+1 is the index of cell one position right on the grid.
		
		//If cell has been reached from West, set new value of cell as the gridcell on the grid, one position left to present cell
		else if (cell.predecessor == 'W')
			cell = grid[cell.y][cell.x - 1];
			//(cell.x-1) is index of present cell on grid. (Cell.x-1)-1 is the index of cell one position left on the grid
	}

	//Now cell object holds the source cell.. Write source cell paramaters on the solution.txt
	solution << 1 << " " << cell.x << " " << cell.y << endl; 
	solution << 0; //Last line holds 0 to mark the end of solution
	solution.close();

}


void main()
{
	//Wavefront:Creating a minheap of gridcells indexed on their pathcost
	priority_queue <gridcell, vector<gridcell>, compare_pathcost> wavefront;
	
	//2D vector called grid holds grid cells
	vector<vector<gridcell>> grid = create_grid();


	gridcell destinationcell = read_source_and_destination(grid, wavefront);
	

	wavefront_expansion(wavefront, grid, destinationcell);

	backtrace(grid, destinationcell);

	
}

