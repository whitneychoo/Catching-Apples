//Whitney Choo
//So Yon Kwon

//Catching Apples

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"

typedef struct Apple
{
  int xpos;
  int ypos;
  int velocity;
  struct Apple *next;
} applenode;

typedef struct Bomb
{
  int bxpos;
  int bypos;
  int bvelocity;
  struct Bomb *bnext;
} bombnode;			//struct for white circular bomb


typedef struct Dia
{
  int dxpos;
  int dypos;
  int dvelocity;
  struct Dia *dnext;
} dianode;			//struct for blue diamond shape

dianode *createdia (dianode *);
void resetbomb (bombnode *);
void resetapple (applenode *, int *);
void resetdia (dianode *);
void increment (applenode *, int *, bombnode *, dianode *);
applenode *createnodes (applenode *);
bombnode *createbomb (bombnode *);
void drawobject (applenode *, bombnode *, dianode *);
void createbasket (int *, int);
int checkappleposition (applenode *, int, int, int *, bombnode *, dianode *);
void makestring (char[], int);
void gameover (void);
void scorekeeper (int, int);
void home ();

int
main ()
{
  int n;
  srand (time (NULL));
  applenode *p, *head = NULL;
  bombnode *q, *bhead = NULL;
  dianode *r, *dhead = NULL;
  int basketx, *basketpointer;	// x coordinates of basket
  char c;
  int applecount = 0, applerelease = 4;	// counter of apples caught,counter of apples released
  int *applepointer;

  applepointer = &applerelease;
  basketpointer = &basketx;
  *basketpointer = 300;
  *applepointer = 4;

  gfx_open (800, 618, "Apple Catch");
  head = createnodes (head);
  bhead = createbomb (bhead);
  dhead = createdia (dhead);
  drawobject (head, bhead, dhead);

  gfx_clear ();

  home ();
  gfx_flush ();

  c = gfx_wait ();		//wait for keyboard command from user
  if (c == 'c')
    {
      while (1)
	{
	  if (applerelease > 104)
	    {			// allows the player to catch 100 apples max
	      gameover ();
	      if (gfx_event_waiting ())
		{
		  c = gfx_wait ();	//wait for keyboard command from user
		  if (c == 'q')
		    return 0;
		}
	    }
	  else
	    {
	      gfx_flush ();
	      usleep (30000);
	      gfx_clear ();
	      increment (head, applepointer, bhead, dhead);
	      drawobject (head, bhead, dhead);
	      createbasket (basketpointer, applecount);
	      scorekeeper (applecount, applerelease);
	      applecount =
		checkappleposition (head, applecount, basketx,
				    applepointer, bhead, dhead);
	    }
	  if (gfx_event_waiting ())
	    {
	      c = gfx_wait ();

	      if (c == 'a')
		*basketpointer = *basketpointer - 30;	//moves basket to the right
	      if (c == 's')
		*basketpointer = *basketpointer + 30;	//to the left
	      if (c == 'q')
		return 0;
	    }
	}
    }

}


void
home (void)
{
  //create the homepage
  char t[50] = "Welcome to Catching Apples!";
  char t1[100] = "Try to catch as many as you can";
  char t2[50] = "The round will end after 100 fallen apples.";
  char t3[100] = "Make sure you keep your eyes out for";
  char t4[50] = "the white bombs and blue squares.";

  char t5[100] = "Press c to continue";

  char t6[50] = "Red Apple: +1pt";
  char t7[50] = "White Bomb: -2pt";
  char t8[50] = "Blue power bar: +4pt";

  change_font ("12x24");
  gfx_text (100, 50, t);
  gfx_color (102, 178, 255);
  gfx_text (100, 120, t1);
  gfx_text (100, 160, t2);
  gfx_text (100, 200, t3);
  gfx_text (100, 240, t4);

  gfx_color (0, 255, 255);

  gfx_text (100, 320, t6);
  gfx_text (100, 360, t7);
  gfx_text (100, 400, t8);


  gfx_color (255, 255, 255);
  gfx_text (100, 500, t5);
}

void
gameover (void)
{
  char gameover[20] = "Game Over";
  gfx_color (255, 255, 255);

  change_font ("12x24");
  gfx_text (200, 250, gameover);
}

void
scorekeeper (int applecount, int applerelease)
{
  char score[4];
  char total[4];
  char text[100] = "Score:";
  char text2[10] = " out of ";
  char text3[20] = "Apple: +1pt";
  char text4[20] = "White Bomb: -2pt";
  char text5[20] = "Blue Bar:+4pt";

  gfx_color (255, 255, 255);
  change_font ("12x24");

  gfx_text (650, 100, text);
  makestring (score, applecount);
  gfx_text (673, 150, score);
  gfx_text (640, 180, text2);
  if (applerelease > 100)
    {
      applerelease = 100;
    }
  makestring (total, applerelease);

  gfx_text (673, 210, total);
  change_font ("6x12");
  gfx_text (640, 400, text3);
  gfx_text (640, 450, text4);
  gfx_text (640, 500, text5);
  gfx_flush ();

}

int
checkappleposition (applenode * p, int applecount, int basketx,
		    int *applepointer, bombnode * q, dianode * r)
{
  while (p)
    {
      if (((p->ypos >= 558) && (p->ypos <= 590)) && ((p->xpos > basketx) &&
						     (p->xpos <
						      (basketx + 80))))
	{
	  applecount = applecount + 1;	// increment the apple count
	  resetapple (p, applepointer);	// reset apple so it doesn't overcount
	}
      p = p->next;
    }
  while (r)
    {
      if (((r->dypos >= 558) && (r->dypos <= 590)) && ((r->dxpos > basketx)
						       &&
						       (r->dxpos <
							(basketx + 80))))
	{
	  resetdia (r);		// reset dia so it doesn't overcount

	  applecount = applecount + 4;	//increase by 4
	}
      r = r->dnext;
    }

  while (q)
    {
      if (((q->bypos >= 558) && (q->bypos <= 590))
	  && ((q->bxpos > basketx) && (q->bxpos < (basketx + 80))))
	{
	  if (applecount >= 2)
	    {
	      applecount = applecount - 2;	// decrease the apple count
	      resetbomb (q);	// reset apple so it doesn't overcount
	    }
	  if (applecount <= 1)
	    {
	      applecount = 0;
	      resetbomb (q);
	    }
	}
      q = q->bnext;
    }

  return applecount;		//returns the # of apples caught

}

void
makestring (char apples[], int applecount)
{

  int i, rem, len = 0, n;

  n = applecount;
  while (n != 0)
    {
      len++;
      n /= 10;
    }
  for (i = 0; i < len; i++)
    {
      rem = applecount % 10;
      applecount = applecount / 10;
      apples[len - (i + 1)] = rem + '0';
    }
  apples[len] = '\0';
}


void
createbasket (int *basketpointer, int applecount)
{
  char apples[3];

  makestring (apples, applecount);	// convert int to string

  gfx_color (139, 69, 19);
  change_font ("12x24");

  if (*basketpointer <= 0)
    {
      gfx_fill_rectangle (0, 580, 80, 30);
      *basketpointer = 0;
    }
  else if (*basketpointer >= 560)
    {
      gfx_fill_rectangle (560, 580, 80, 30);
      *basketpointer = 560;
    }
  else
    {
      gfx_fill_rectangle (*basketpointer, 580, 80, 30);	// make basket
    }

  gfx_color (0, 0, 0);
  gfx_text (*basketpointer + 28, 605, apples);	// print score on basket

  gfx_flush ();
}

void
resetapple (applenode * p, int *applepointer)
{
  p->xpos = rand () % 500 + 20;	// re-randomize x position
  p->ypos = 25;			// reset apple to start from top
  p->velocity = rand () % 10 + 3;	// re-randomize velocity
  *applepointer = *applepointer + 1;

}

void
resetdia (dianode * r)
{
  r->dxpos = rand () % 500 + 20;	// re-randomize x position
  r->dypos = 25;		// reset dia to start from top
  r->dvelocity = rand () % 10 + 6;	// re-randomize velocity
}

void
resetbomb (bombnode * q)
{
  q->bxpos = rand () % 500 + 20;	// re-randomize x position
  q->bypos = 25;		// reset apple to start from top
  q->bvelocity = rand () % 10 + 5;	// re-randomize velocity
}

applenode *
createnodes (applenode * head)
{
  applenode *p;
  int n;
  for (n = 1; n <= 4; n++)
    {
      p = (applenode *) malloc (sizeof (applenode));	// create a new node
      p->xpos = rand () % 500 + 50;	// randomize the xpos
      p->ypos = 20;
      p->velocity = rand () % 10 + 3;	// randomize velocity
      p->next = head;
      head = p;			// set next to the head pointer
    }

  return head;
}

bombnode *
createbomb (bombnode * bhead)
{
  bombnode *q;
  int n;

  for (n = 1; n <= 2; n++)
    {
      q = (bombnode *) malloc (sizeof (bombnode));	// create a new node
      q->bxpos = rand () % 500 + 50;
      q->bypos = 20;
      q->bvelocity = rand () % 10 + 5;
      q->bnext = bhead;
      bhead = q;
    }

  return bhead;
}

dianode *
createdia (dianode * dhead)
{
  dianode *r;
  int n;

  for (n = 1; n <= 1; n++)
    {
      r = (dianode *) malloc (sizeof (dianode));	// create a new node
      r->dxpos = rand () % 500 + 50;
      r->dypos = 20;
      r->dvelocity = rand () % 10 + 6;
      r->dnext = dhead;
      dhead = r;
    }

  return dhead;
}


void
drawobject (applenode * p, bombnode * q, dianode * r)
{
  int i;
  while (p)
    {
      gfx_color (255, 0, 0);	// make red
	    gfx_circle (p->xpos, p->ypos, 25);	// create circle
      gfx_color (255, 255, 255);	// make white
      gfx_line (p->xpos, p->ypos - 26, p->xpos, p->ypos - 33);
      gfx_color (0, 189, 0);	// make green
      gfx_triangle (p->xpos, p->ypos - 33, p->xpos + 16, p->ypos - 43,
		    p->xpos + 25, p->ypos - 36);
      gfx_flush ();
      p = p->next;
    }

//draw bomb
  while (q)
    {
      gfx_color (224, 224, 224);
  	  gfx_circle (q->bxpos, q->bypos, 20);	// create circle
      gfx_color (0, 0, 0);
      gfx_line (q->bxpos - 10, q->bypos - 10, q->bxpos + 10, q->bypos + 10);
      gfx_line (q->bxpos + 10, q->bypos - 10, q->bxpos - 10, q->bypos + 10);
      gfx_flush ();
      q = q->bnext;
    }


//draw dia
  while (r)
    {
      gfx_color (0, 0, 255);
      gfx_fill_rectangle (r->dxpos, r->dypos, 12, 12);
      gfx_flush ();
      r = r->dnext;
    }

}

void
increment (applenode * p, int *applepointer, bombnode * q, dianode * r)
{
  while (p)
    {
      p->ypos = p->ypos + p->velocity;	// increment ypos for movement
      if (p->ypos > 650)
	{			// if ypos is too low on the screen
	  resetapple (p, applepointer);	// reset the ypos
	}
      p = p->next;
    }

  while (q)
    {
      q->bypos = q->bypos + q->bvelocity;	// increment ypos for movement
      if (q->bypos > 650)
	{
	  resetbomb (q);	//reset bomb ypos
	}
      q = q->bnext;
    }

  while (r)
    {
      r->dypos = r->dypos + r->dvelocity;	// increment ypos for movement
      if (r->dypos > 650)
	{
	  resetdia (r);		//reset bomb ypos
	}
      r = r->dnext;
    }


}
