/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include "dbTable.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
   // TODO: to print out a row.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(int i = 0; i < r.size()-1;i++)
   {
       os<<((r[i]==INT_MAX)?'.':r[i])<<" ";
   }
   os<<r[r.size()-1]<<endl;
   return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // TODO: to print out a table
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells are printed as '.'
   for(int i =0;i<t.nRows();i++)
   {
       for(int j=0;j<t.nCols();j++)
       {
           if(t[i][j]==INT_MAX)
           {
               os<<setw(6)<<right<<".";
           }else{os<<setw(6)<<right<<t[i][j];}
       }
       os<<endl;
   }
   return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
   // TODO: to read in data from csv file and store them in a table
   // - You can assume all the data of the table are in a single line.
   string datastr;
   getline(ifs,datastr);
   string delim ="\r";
   string row_delim=",";
   string row_str;
   size_t begin_pos = 0, end_pos = datastr.find(delim);
   while(end_pos!=begin_pos)
   {
       DBRow* row = new DBRow();
       row_str=datastr.substr(begin_pos,end_pos - begin_pos)+",";
       size_t row_begin_pos = 0, row_end_pos =row_str.find(row_delim);
       while(row_end_pos!=string::npos)
       {    
           string value_str = row_str.substr(row_begin_pos,row_end_pos-row_begin_pos);
           row->addData((value_str=="")?INT_MAX:stoi(value_str));
           row_begin_pos = row_end_pos+row_delim.size();
           row_end_pos = row_str.find(row_delim,row_begin_pos);
       }
       t.addRow(*row);
       begin_pos = end_pos+delim.size();
       end_pos = datastr.find(delim,begin_pos);
   }
   return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void
DBRow::removeCell(size_t c)
{
   // TODO
   _data.erase(_data.begin()+c);
   return;
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // TODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder
   for(int i=0;i<_sortOrder.size();i++)
   {
       if(r1[_sortOrder[i]]!=r2[_sortOrder[i]]){return r1[_sortOrder[i]]<r2[_sortOrder[i]];}
   }
   return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
   // TODO
   _table.clear();
   return;
}

void
DBTable::addCol(const vector<int>& d)
{
   // TODO: add a column to the right of the table. Data are in 'd'.
    for(int i = 0;i<nRows();i++ )
    {
        _table[i].addData(d[i]);
    }
    return;
}

void
DBTable::delRow(int c)
{
   // TODO: delete row #c. Note #0 is the first row.
   _table.erase(_table.begin()+c);
}

void
DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (size_t i = 0, n = _table.size(); i < n; ++i)
      _table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
   // TODO: get the max data in column #c
   float max = _table[0][c];
   for(int i = 1;i<nRows();i++)
   {
       max=(max<_table[i][c]&&_table[i][c]!=INT_MAX)?_table[i][c]:max;
   }
   return (max==INT_MAX)?NAN:max;
}

float
DBTable::getMin(size_t c) const
{
   // TODO: get the min data in column #c
   float min = _table[0][c];
   for(int i =1 ;i<nRows();i++)
   {
       min = (min>_table[i][c])?_table[i][c]:min;
   }
   return (min==INT_MAX)?NAN:min;
}

float 
DBTable::getSum(size_t c) const
{
   // TODO: compute the sum of data in column #c
   int flag = 0;
   float sum = 0;
   for(int i =0;i<nRows();i++)
   {
       sum+=(_table[i][c]==INT_MAX)?0:_table[i][c];
       flag = (_table[i][c]!=INT_MAX)?1:flag;
   }
   return (flag==0)?NAN:sum;
}

int
DBTable::getCount(size_t c) const
{
   // TODO: compute the number of distinct data in column #c
   // - Ignore null cells
   int count =0;
   for(int i =0;i<nRows();i++)
   {
       count = (_table[i][c]==INT_MAX)?count:count+1;
   }
   return count;
}

float
DBTable::getAve(size_t c) const
{
   // TODO: compute the average of data in column #c
   return (getCount(c)==0)?NAN:getSum(c)/getCount(c);
}

void
DBTable::sort(const struct DBSort& s)
{
   // TODO: sort the data according to the order of columns in 's'
   std::sort(_table.begin(),_table.end(),s);
   return;  
}

void
DBTable::printCol(size_t c) const
{
   // TODO: to print out a column.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(int i =0;i<nRows();i++)
   {
       if(_table[i][c]==INT_MAX)
       {
           cout<<setw(2)<<left<<".";
       }else{cout<<setw(2)<<left<<_table[i][c];}
   }
   cout<<endl;
   return;
}

void
DBTable::printSummary() const
{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}

