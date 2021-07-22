# PrettyListingRemover
"Pretty listing" remover
-------------------------------------------------------------------
I recently was porting some code from VS2008 to VS2015. 

I was ready for incredibly slow Intellisense which is the stupidest piece of 
software I ever met which reinitilises for minutes blocking your work or displays modal "Wait..." window while 
looking for a function definition in the same file. But I did not expect that its text editor could be made so 
intrusive by correcting your every step. It keeps saying "you are stupid and I will help you" every time when I open a 
bracket; it inserts a closing one by its side despite I never asked for that. So every time I need to remove the
second one and type a closing bracket at a proper place. Loooots of such rubbish. If you think that 
it is only needed to turn off "autocompletion" somewhere in Options, you are wrong. You must watch for "brackets" in Options
but you will not find any. There are dozens of places which control various autocompletions and actions and other similar 
operations, on paste, on typing, on indentation etc. 

But this is not the whole story. There is such a terrible thing called <B>reformatting</B> controlled in Options and not from a single place. 
While porting the code, I wrote a console application to tidy up the old code, namely place classes into separate files, insert Doxygen-style comments,
divide the whole thing into namespaces etc. and wrote a special console application to do this
(the whole code is quite large). I made the new code compile under 2015 but at some moment after some work with it I discovered that <B>THE WHOLE CODE</B> had been reformatted without any warning. I have no idea when. Now I know this reformatting is called "pretty listing". 

So I needed to find a way how to get rid of the changes. This simple console application removes some "pretty" listing features according to my preferences because it is hard to do it manually where multiline replacements in many files are needed.

My preferences
--------------
A computer code must save space to show as much data on computer screen as possible. It means that I do not like multiple blank lines
separating code fragments - blank lines do not carry information. I do not like such construction as three-line "else" like

	}
	else
	{

I prefer

	} else
	{
	
Originally I wanted to make only this change.
	
I do not like "if(" without space.

I prefer

  CXGeometry *list = new CXGeometry();

  // get list of spline surfaces
  CXGeometry *splinesurfaces = new CXGeometry();

  // get cross-shape info
  int tubemargins[2], tubeC1indexes[4], ductC1indexes[4];
  GetShapePatchInfo(shapetype,tubemargins,tubeC1indexes,ductC1indexes);

to
  
  CXGeometry *list = new CXGeometry();
  // get list of spline surfaces
  CXGeometry *splinesurfaces = new CXGeometry();
  // get cross-shape info
  int tubemargins[2], tubeC1indexes[4], ductC1indexes[4];
  GetShapePatchInfo(shapetype,tubemargins,tubeC1indexes,ductC1indexes);
  
but even better to place comments from position 31 and get an even better look

  CXGeometry *list = new CXGeometry();
								// get list of spline surfaces
  CXGeometry *splinesurfaces = new CXGeometry();
								// get cross-shape info
  int tubemargins[2], tubeC1indexes[4], ductC1indexes[4];
  GetShapePatchInfo(shapetype,tubemargins,tubeC1indexes,ductC1indexes);

PrettyListingRemover.cpp
------------------------
  
So please have a look at the PrettyListingRemover.cpp. The code takes a file (or files one by one), copies orginal version into a .bak, makes some replacements/removals and stores modified file with the same name. Every operation is explained and can be disabled by setting 

	#if 0 
	
at every code fragment (easy to find). You can add any new operation according to your preferences.
You invoke the program like

	>PrettyListingRemover CMesh.cpp

or by

	>PrettyListingRemover C:\MyProjects\Mesh\*.cpp
	
(full path mask is required).

Language
--------
C++, VS 2019

P.S.
----

To be fair, Visual Studio has a very good debugger and profiler (profiler in recent versions). Optimiser is good but sometimes makes mistakes.
It happens.

