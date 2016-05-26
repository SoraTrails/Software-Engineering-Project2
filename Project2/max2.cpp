#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define M 20
#define BLOCKNUM 3
#define STEPSIZE 0.005

typedef struct _circle {
	//int order;
	double x;
	double y;
	double r;
	_circle *next;
}CIRCLE;

typedef struct _ball {
	//int order;
	double x;
	double y;
	double z;
	double r;
	_ball *next;
}BALL;

typedef struct _block2
{
	double x;
	double y;
}BLOCK2;

typedef struct _block3
{
	double x;
	double y;
	double z;
}BLOCK3;

CIRCLE* circlesHead = new CIRCLE;
BALL* ballsHead = new BALL;
BLOCK2 blocks2[BLOCKNUM];
BLOCK3 blocks3[BLOCKNUM];

void produceBlock()
{
	srand(time(NULL));
	for (int i = 0;i < BLOCKNUM;i++)
	{
		blocks2[i].x = (double)((double)rand() / RAND_MAX) * 2 - 1;
		blocks2[i].y = (double)((double)rand() / RAND_MAX) * 2 - 1;
		blocks3[i].x = (double)((double)rand() / RAND_MAX) * 2 - 1;
		blocks3[i].y = (double)((double)rand() / RAND_MAX) * 2 - 1;
		blocks3[i].z = (double)((double)rand() / RAND_MAX) * 2 - 1;
	}
}

bool ifOverlap2(double x, double y, double r)
{
	//if (x + r > 1 || x - r < -1	|| y + r > 1 || y - r < -1)
	//	return false;
	CIRCLE *temp = circlesHead->next;

	while (temp != NULL)
	{
		if (temp->r + r > sqrt((temp->x - x)*(temp->x - x) + (temp->y - y)*(temp->y - y)))
			return false;
		temp = temp->next;
	}
	return true;
}
bool ifOverlap3(double x, double y, double z, double r)
{
	//if (x + r > 1 || x - r < -1 || y + r > 1 || y - r < -1 || z + r > 1 || z - r < -1)
	//	return false;
	BALL *temp = ballsHead->next;

	while (temp != NULL)
	{
		if (temp->r + r > sqrt((temp->x - x)*(temp->x - x) + (temp->y - y)*(temp->y - y) + (temp->z - z)*(temp->z - z)))
			return false;
		temp = temp->next;
	}
	return true;
}

void max2d()
{
	CIRCLE *add = new CIRCLE, *temp;
	add->r = 1;
	add->x = 0;
	add->y = 0;
	add->next = NULL;
	circlesHead->next = add;

	for (int i = 1;i < M;i++)
	{
		for (double r = 0.180;r > 0;r -= STEPSIZE)
		{
			for (double x = -1.0 + r;x < 1.0 - r;x += STEPSIZE)//三四象限
				for (double y = -1.0 + r;y <(-1) * sqrt(1 - x * x);y += STEPSIZE)
				{
					if (ifOverlap2(x, y, r))
					{
						temp = new CIRCLE;
						temp->r = r;
						temp->x = x;
						temp->y = y;
						temp->next = NULL;
						add->next = temp;
						add = add->next;
						goto out;
					}
				}
			for (double x = -1.0 + r;x < 1.0 - r;x += STEPSIZE)//一二象限
				for (double y = sqrt(1 - x * x);y < 1.0 - r;y += STEPSIZE)
				{
					if (ifOverlap2(x, y, r))
					{
						temp = new CIRCLE;
						temp->r = r;
						temp->x = x;
						temp->y = y;
						temp->next = NULL;
						add->next = temp;
						add = add->next;
						goto out;
					}
				}

		}
		printf("error2\n");
		break;
	out:
		continue;
	}

}

void max3d()
{
	BALL *add = new BALL, *temp;
	add->r = 1;
	add->x = 0;
	add->y = 0;
	add->z = 0;
	add->next = NULL;
	ballsHead->next = add;

	for (int i = 1;i < M;i++)
	{
		for (double r = 0.27;r > 0;r -= STEPSIZE)
		{
			for (double x = -1.0 + r;x < 1.0 - r;x += STEPSIZE)//下半空间
				for (double y = -1.0 + r;y < 1.0 - r;y += STEPSIZE)
					for (double z = -1.0 + r;z <(-1) * sqrt(1 - x * x - y * y);z += STEPSIZE)
					{
						if (ifOverlap3(x, y, z, r))
						{
							temp = new BALL;
							temp->r = r;
							temp->x = x;
							temp->y = y;
							temp->z = z;
							temp->next = NULL;
							add->next = temp;
							add = add->next;
							goto out;
						}
					}
			for (double x = -1.0 + r;x < 1.0 - r;x += STEPSIZE)//上半空间
				for (double y = -1.0 + r;y < 1.0 - r;y += STEPSIZE)
					for (double z = sqrt(1 - x * x - y * y);z <1.0 - r;z += STEPSIZE)
					{
						if (ifOverlap3(x, y, z, r))
						{
							temp = new BALL;
							temp->r = r;
							temp->x = x;
							temp->y = y;
							temp->z = z;
							temp->next = NULL;
							add->next = temp;
							add = add->next;
							goto out;
						}
					}
		}
		printf("error3\n");
		break;
	out:
		continue;
	}

}

int main()
{
	clock_t start2, end2, start3, end3;
	start2 = clock();
	max2d();
	end2 = clock();

	produceBlock();

	CIRCLE *temp1 = circlesHead->next;
	double sum1 = 0;
	while (temp1 != NULL)
	{
		sum1 += temp1->r*temp1->r;
		temp1 = temp1->next;
	}
	printf("sum2(r^2):%lf\n", sum1);
	printf("time2:%.3f\n", (double)(end2 - start2) / CLOCKS_PER_SEC);
	system("pause");


	start3 = clock();
	max3d();
	end3 = clock();

	BALL *temp2 = ballsHead->next;
	double sum2 = 0;
	while (temp2 != NULL)
	{
		sum2 += temp2->r*temp2->r;
		temp2 = temp2->next;
	}
	printf("sum3(r^2):%lf\n", sum2);
	printf("time3:%.3f\n", (double)(end3 - start3) / CLOCKS_PER_SEC);
	system("pause");
}