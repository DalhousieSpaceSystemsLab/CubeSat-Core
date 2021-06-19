# LORIS C Standard Proposal
Version 1.0

## Purpose 
The purpose of this document is to define some C programming standards the LORIS CubeSat project may follow.

The advantages of subscribing to standardized coding practice include:
- Better code readability and sharing;
- Prevention of common issues that are difficult to diagnose;
- Simplifies code integration, testing and error handling;

## Quick summary
1. [Use automatic code formatting](#1-automatic-code-formatting)
2. [Treat headers like they're special](#5-headers-are-special)
3. [Abolish global variables](#3-abolish-global-variables)
4. [No more run & pray; catch function return values](#2-no-more-run--pray)
5. [Easy on the malloc](#4-away-with-malloc)

## 1. Automatic code formatting
Starting off, there exist tools which will automatically format your code as you program. Notably is the clang-formatter, which has already been setup for the core software.

### This-way-or-that-way
One advantage of using an automatic code formatter is that it automatically takes care of all of the little trivial decisions programmers often have to make. An example would be whether to use same-line opening brackets or newline opening brackets:
```C
int main() {

}

// VS 

int main()
{

}
```

Indeed a timeless debate, but fundamentally doesn't matters as long as it is applied consistently. 

The key here is that the automatic formatter, once set, will always set it one way or the other for everyone. This way consistency is guaranteed and time isn't wasted deciding how to do it.

### Superhuman readability
Another thing which automatic code formatting offers is the ridiculously meticulous arrangement of small details which go a long way in making the code look readable and nice.

An example of this would be comment alignment. Consider the following:
```C
struct human {
  int age; // age in years
  float height; // height in m
  float weight; // weight in kg
  char* name; // first name + last name
  char* maiden_name; // mother's maiden name
};

// VS 

struct human {
  int   age;          // age in years
  float height;       // height in m
  float weight;       // weight in kg
  char* name;         // first name + last name
  char* maiden_name;  // mother's maiden name
};
```

It's a good idea to include comments next to the elements of a struct as it provides more context about their usage. What's less of a good idea is to spend tons of time aligning all of the comments as a formatter would do in 0.002 seconds. 

## 2. Headers are special 
The C compiler is flexible in allowing the programmer to configure their code in a multitude of ways. When it comes to header file configuration, taking a free-for-all approach can lead to issues down the software integration road.

### Header guards 
Header guards are an old but useful tool in preventing unintentional re-inlcuding of the same headers in your code.

They typically look something like this:
```C
// my_header.h 

#ifndef MY_PROJECT_MY_HEADER_H
#define MY_PROJECT_MY_HEADER_H

// code goes here...

#endif 
```

Basically what the `#ifndef` and `#define` are saying is that if `MY_PROJECT_MY_HEADER_H` has never been defined before, define it and use the code leading up to `#endif`. If it already has been defined before, skip everything. 

This effectively allows us to avoid the re-including of headers in the same source code, since any header with a header guard will be blank if it has delcared its own existence before.

### Delcarations ONLY
Headers are primarly used to share references and definitions between source files. So, there shouldn't be a single variable assignment in a header file.

```C
// my_header.h 

#ifndef MY_PROJECT_MY_HEADER_H
#define MY_PROJECT_MY_HEADER_H

////////////////////////
//  Global variables  //
////////////////////////

int x = 5;    // no 
extern int x; // yes

/////////////////
//  Functions  //
/////////////////

void func() {     // no
  int x = 1 + 1;  // no
}                 // no

void func();      // yes

//////////////////
//  Data types  //
//////////////////

struct human alex = { // no
  .age = 20,          // no
  .height = 184.5,    // no
  .name = "alex",     // no
};                    // no

struct human {      // yes
  int age;          // yes
  float height;     // yes
  const char* name; // yes
};

#endif 
```

## 3. Abolish global variables 
Global variables are very similar to pharmaceutical advertisements: happy people running in a field, problem-free. The simplest global variable can often solve a complex problem.

I must insist on looking at the list of side-effects, however, as they tell a different story.

### Infinite pain 
Global variables find themselves in a bit of a free-for-all position. Depending on how global the scope of the variable was set to be, you could be giving access to every function in a source file or more to read and write to the same variable. 

When a program first starts out, these interactions are relatively simple. But as complexity increases, contradictory interactions may begin to go unchecked, causing unpredictable behavior that is difficult to spot.

### Prevention 
The most effective way to avoid the consequences of global variables is to avoid them whenever possible. 

This is often not trivial to do, as it is common for functions to need a way to share information without being the same function.

#### Let the parent handle it 
One way to get around the need for global variables is moving the placeholder of the information 'up' one level. 

For example, if functions `get_image()` and `scan_image()` both need image data to work, we may be tempted to do this:
```C
FILE* image;

void get_image() {
  image = fopen("img.jpg", "r");
}

void scan_image() {
  while((char c = fgetc(image)) != EOF) {
    // do stuff with image
  }
}
```

Although this setup does allow both functions to easily share the image file, there are many new things which now need to be taken into account. Can `scan_image()` trust that `image` will not suddenly change while it is working? What if `get_image()` overrides a file that was already open with a new file?

To avoid this, the parent calling these functions can store the `FILE` pointer locally, like so:

```C
FILE* get_image() {
  FILE* image = fopen("img.jpg", "r");
  return image;
}

void scan_image(FILE* image) {
  while((char c = fgetc(image)) != EOF) {
    // do stuff with image
  }
}

int main() {
  FILE* image = get_image();
  scan_image(image);
}
```

With only a few additional lines of code, the global variable was abolished. 

### If you're going to do it anyway...
Realistically, a program will become drastically more complicated if global variables are completely eliminated. So when you do get around to making them, the following rules of thumb should help you out.

#### Glocal variables 
In C/C++, it is possible to create global variables that are only global within a source file. 

These can be made using the `static` keyword when declared outside of a function body, like so:

```C
// my_source.c

static int status = 0;
static bool restarted = false;

int main() {
  ...
}
```

In this case, every function within the source code may use them, but it stops there. They are not visible from the outside, and are basically 'private' variables. 

#### Extern 
If a global variable needs to be shared between source files, this may be done using the `extern` keyword. It is a little counter-intuitive, so bear with me.

First, the variable must be declared in the header file with the `extern` keyword, like so:
```C
// my_header.h 

extern int status;
```

Despite how it looks, we're only declared that somewhere in the code, there should be an `int status`. The variable hasn't actually been created, so there is nowhere for the information to go.

Next, the actual variable must be created in a source file, like so:
```C
// my_source.c 

int status;
```

At this point, the `status` variable has been setup as a global variable and may now be used anywhere `my_header.h` is included.

#### Interacting with global variables safely 
It can be very helpful to avoid directly interacting with global variables, as sometimes some additional steps are required to make sure it is safe to use them. 

For this to work, you can create getter/setter functions which act as the custodians of your global variables. This way, the global variables are only modified in very specific and controlled ways. 

```C
static int status = 0;
static bool busy = false;

int get_status() {
  if(!busy) {
    return status;
  } else {
    return -1;
  }
}

void set_status(int new_stat) {
  status = new_stat;
}

bool get_busy_state() {
  return busy;
}

void set_busy_state(bool new_busy) {
  busy = new_busy;
}
```

If the free-for-all approach is used, you are likely going to access/set the global variables in different ways every time. So using getters/setters helps to set a cap on how complicated interacting with the globals can become.

## 4. No more run & pray 
Although in the C++ world there is a built-in exception handling system, C does not offer such a thing. As such, the only way to know if a function was run successfully is to check the return value of the function (or in some cases, a dedicated error variable).

It is often tempting to skip this, especially when your code already works. But when something inevitably goes wrong, you are left in the dark.

What's more, the environment the satellite will find itself in is completely isolated from us, the developers. 

So for error handling to be thoroughly automated, every function call needs to be checked.

### What it looks like 
Other than in a few exceptional cases, every function call should look something like this:
```C
if(foo() != 0) {
  printf("foo did not work wah wah\n");
  return -1;
}
```

Every function has a different way of announcing an error, so it will not look exactly like this. But the main jist of it is to wrap or 'catch' every function return value. 

### Need to keep the value
In some cases, the return value is still needed after the function is run. When this happens, there are 2 ways the error can be caught:
```C
int bytes_read = read(something);
if(bytes_read < 0) {
  printf("failed to read\n");
  return -1;
}

// OR 

int bytes_read;
if((bytes_read = read(something)) < 0) {
  printf("failed to read\n");
  return -1;
}
```

The exact approach you take is up to you. In either case, the function call is not given the benefit of the doubt and the program does not continue without confirming a successful run.

### Making it easier 
It can quickly become cubersome to wrap a ton of `if` statements around every function call. Fortunately there are tricks to make this easier and more convenient. 

As described in the [MACROS.md](/md_src_MACROS.html) article, there exist a couple macros which help us achieve this with much less clutter.

#### The OK and ON_FAIL macros
The `OK(function)` macro wraps the provided function call in an `if` statement which checks to see if the return value was less than 0:

Similarly, the `ON_FAIL()` macro wraps a function call with an `if` statement to check if the return value is less than 0. Except it allows you to define a custom action in case of failure.

```C
OK(func());

// OR 

ON_FAIL(func(), return -1);

// EQUIVALENT TO 

if (func() < 0) {                 
  moderr("%s failed\n", #func); 
  return -1;                    
}
```

As you can see, these options result in much less clutter without losing the advantage of checking every function call.

More on this in [MACROS.md](/md_src_MACROS.html).



## 5. Easy on the malloc
Don't get me wrong, there are many advantages to using dynamic memory. Namely, the amount of allocated memory may be changed at any time, in much larger amounts and anywhere in the program.

However, the situations which necessarily call for using `malloc()` are few and it is best avoided when possible.

### When to use malloc
The most common use cases for `malloc()` would be:
- Storing a large amount of data in an array (1MB+ file),
- Information which needs to be stored for the lifetime of the program (or a long while),
- Information which needs to be accessed from 'far away', where it would be complicated to pass it via parents.

Otherwise, there likely exists a way to use automatic/static allocation. There are always exceptions though.

### Getting around malloc 
Instead of using `malloc()`, static memory allocation can be used. 

This includes any kind of automatically allocated arrays, such as:
```C
int ages[25];
float temperatures[10];
```

The advantage of sticking to static memory allocation whenever possible is that it is automatically managed. Whenever the array enters or leaves the program scope, it is automatically loaded/removed from memory.

The disadvantage of using static memory allocation is that the size needs to be known ahead of time. This can be tricky when dealing with data that may be of variable size.

The simplest way around this is to define a `MAX_ARRAY_SIZE` constant which is used around your code. As long as it is big enough to work in almost every use case, then it works like a charm.

```C
#define MAX_NUM_AGES  25
#define MAX_NUM_TEMPS 10

int ages[MAX_NUM_AGES];
float temperatures[MAX_NUM_TEMPS];
```

## Final thoughts 
If there is any way this standard could be improved, please let me know what you think. 