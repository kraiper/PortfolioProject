#ifndef _CONSTANTS__H
#define _CONSTANTS__H


static const int NROFLIGHTS = 3;
static const int NRTRIANGLES = 12;
static const int NRTRIANGLESMESH = 12;
static const int NRLIGHTSUSE = 1;
static const int NROFBOUNCES = 10;

static const int CORETHREADSWIDTH = 32; // dont change
static const int CORETHREADSHEIGHT = 32; // dont change
static const int COREMULTIPLIERWIDTH = 32; // change to modify screen size
static const int COREMULTIPLIERHEIGHT = 32; // change to modify screen size

static const float WIDTH = CORETHREADSWIDTH * COREMULTIPLIERWIDTH;
static const float HEIGHT = CORETHREADSHEIGHT * COREMULTIPLIERHEIGHT;

static const int NROFTHREADSWIDTH = int(WIDTH / CORETHREADSWIDTH);
static const int NROFTHREADSHEIGHT = int(HEIGHT / CORETHREADSHEIGHT);

static const int NROFTREADSKDTREECREATION = 100;

static const int NROFTHREADSCREATIONDISPATCHES = NROFTREADSKDTREECREATION * CORETHREADSWIDTH * CORETHREADSHEIGHT;

static const int CREATIONHEIGHT = NROFTREADSKDTREECREATION * CORETHREADSHEIGHT;

static const int NUMDIM = 3; // the number of dimensions our application uses

#define MAXSIZE 3000000

#define LEFT -1
#define RIGHT 1
#define MIDDLE 0
#define MAXDIST 9999
#define EPSILON 0.0000001
#define PI (3.14159265358979323846f)


#endif