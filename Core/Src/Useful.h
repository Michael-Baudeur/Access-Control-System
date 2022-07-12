#ifndef USEFUL_H
#define USEFUL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>

char* BoolToString(bool b) { return b ? "true" : "false"; }

//Renvoie 1/sqrt(x) pour x€[0, 1]
float OneOverSqrt(float x)
{
    if(x < 0 || x > 1)
        return 1.0/sqrtf(x);

	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = x * 0.5F;
	y  = x;
	i  = * (long*)&y;
	i  = 0x5f3759df - (i >> 1);
	y  = *(float*) &i;
	y  = y * (threehalfs - (x2 * y * y));   // 1st iteration
    y  = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed
	return y;
}

//Renvoie le max(a, b)
int Max(int a, int b)
{
    return a >= b ? a : b;
}
//Renvoie le min(a, b)
int Min(int a, int b)
{
    return a <= b ? a : b;
}
//Renvoie |a|
int Abs(int a)
{
    return a >= 0 ? a : -a;
}
//Renvoie le max(a, b)
int Maxf(float a, float b)
{
    return a >= b ? a : b;
}
//Renvoie le min(a, b)
int Minf(float a, float b)
{
    return a <= b ? a : b;
}
//Renvoie |a|
int Absf(float a)
{
    return a >= 0 ? a : -a;
}
//Renvoie true si la valeur en param est dans le tableau
bool ArrContain(int arr[], int length, int value)
{
    for (int i = 0; i < length; i++)
    {
        if(arr[i] == value)
            return true;
    }
    return false;
}
//Ecrit le tableau dans la console
void ShowArr(int arr[], int n)
{
    printf("[");
    for (int i = 0; i < n - 1; i++)
    {
        printf("%i,", arr[i]); 
    }
    printf("%i]\n", arr[n - 1]);
}

//Ecrit un tableau d'entier dans un fichier texte
void WriteArray(int arr[], int length, char* fileName)
{
    FILE* file = NULL;
    file = fopen(fileName, "w+");
    fputs((char*)"[", file);
    for (int i = 0; i < length - 1; i++)
    {
        char number[10] = "";
        itoa(arr[i], number, 10);
        fputs(number, file);
        fputs((char*)",", file);
    }
    char number2[10] = "";
    itoa(arr[length - 1], number2, 10);
    fputs(number2, file);
    fputs((char*)"]", file);
    fclose(file);
}
//Ecrit un tableau de nombre à virgule dans un fichier texte.
void WriteFloatArray(float arr[], int length, char* fileName)
{
    FILE* file = NULL;
    file = fopen(fileName, "w+");
    fputs((char*)"[", file);
    for (int i = 0; i < length - 1; i++)
    {
        char number[10] = "";
        gcvt(arr[i], 6, number);//on converti un float en char*, 6 pour le nombre de chiffre max dans la chaine de char
        fputs(number, file);
        fputs((char*)",", file);
    }
    char number2[10] = "";
    gcvt(arr[length - 1], 6, number2);
    fputs(number2, file);
    fputs((char*)"]", file);
    fclose(file);
}
//Renvoie un entier uniformément dans [a,b]
int RandInt(int a, int b)
{
    return rand()%(b - a + 1) + a;
}
//Renvoie un flottant uniformément dans [0,1]
float RandFloat(float a, float b)
{
    return ((float)rand() / (float)RAND_MAX) * (b - a) + a;
}
//Randomise la graine d'aléatoire
void SetRandomSeed()
{
    srand(time(NULL));
}
//Renvoie le plus grand diviseur commun entre a et b
int PGCD(int a, int b)
{
    int r;
    while(b != 0)
    {
        r = a%b;
        a = b;
        b = r;
    }
    return a;
}
//Renvoie x^n ou n€|N
int Pow(int x, int n)
{
    int result = 1;
    for(int i = 0; i < n; i++)
    {
        result *= x;
    }
    return result;
}
//Inverse les indices d'un tableau
void InverseArr(int arr[], int length)
{
    for (int i = 0; i < length/2; i++)
    {
        int tmp = arr[i];
        arr[i] = arr[length - 1 - i];
        arr[length - 1 - i] = tmp;
    }
}
//Echange deux éléments d'un tableau
void Switch(int arr[], int i, int j)
{
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}
//private
void QuickSort(int arr[], int first, int last)
{
    if(first >= last) { return; }

    int i = first;
    int j = last;
    int pivot = arr[first];
    while(i < j)
    {
        while(arr[i] <= pivot && i < last)
        {
            i++;
        }
        while(arr[j] > pivot)
        {
            j--;
        }
        if(i < j)
        {
            Switch(arr, i, j);
        }
    }
    Switch(arr, first, j);
    QuickSort(arr, first, j - 1);
    QuickSort(arr, j + 1, last);
}
//Tri le tableau
void Sort(int arr[], int n, bool growOrder)
{
    QuickSort(arr, 0, n - 1);
    if(!growOrder)
    {
        InverseArr(arr, n);
    }
}

//Les structs pour les listes!
typedef struct item
{
    char value;
    struct item *following;
}Item;

typedef struct List
{
    int count;
    //private
    struct item *root;
}List;

//Renvoie une liste vide
List* ListInit()
{
    List* lst = (List*)malloc(sizeof(List));
    lst->root = NULL;
    lst->count = 0;
    return lst;
}

//Une fonction pour crée facilement des nouveau élément de la liste
//private
Item* NewItem(char value)
{
    Item* item = (Item*)malloc(sizeof(Item));
    item->value = value;
    item->following = NULL;
    return item;
}
//Renvoie true si l'index corresponde bien à un élément de la liste
bool ListIndexExist(List* lst, int index)
{
    return index >= 0 && index < lst->count;
}
//Renvoie true si un élément de la liste possède cette valeur.
bool ListContain(List* lst, char value)
{
    if(lst == NULL || lst->root == NULL)
        return false;
    Item* cursor = lst->root;
    while (cursor->value != value && cursor->following != NULL)
    {
        cursor = cursor->following;
    }
    return cursor->value == value;
}
//Renvoie true si tout les éléments du tableau sont dans la liste
bool ListContains(List* lst, char values[], int length)
{
    for (int i = 0; i < length; i++)
    {
        if(!ListContain(lst, values[i]))
            return false;
    }
    return true;
} 
//Ajoute un élément a la fin de la liste
void ListAdd(List* lst, char value)
{
    if(lst == NULL)
    {
        lst = ListInit();
        lst->root = NewItem(value);
        lst->count = 1;
        return;
    }
    if(lst->root == NULL)
    {
        lst->root = NewItem(value);
        lst->count = 1;
        return;
    }
    Item* cursor = lst->root;
    while (cursor->following != NULL)
    {
        cursor = cursor->following;
    }
    cursor->following = NewItem(value);
    lst->count++;
}
//Ajoutes l'ensemble du tableau à la fin de la liste
void ListAdds(List* lst, char values[], int length)
{
    if(length <= 0)
        return;
    for (int i = 0; i < length; i++)
    {
        ListAdd(lst, values[i]);
    }
}
//Ajoute un élément au début de la liste
void ListAddBegining(List* lst, char value)
{
    if(lst == NULL)
    {
        lst = ListInit();
        lst->root = NewItem(value);
        lst->count = 1;
        return;
    }
    Item* newItem = NewItem(value);
    newItem->following = lst->root;
    lst->root = newItem;
    lst->count++;
}
//Ajoute l'ensemble du tableau au début de la liste
void ListAddsBegining(List* lst, char values[], int length)
{
    for (int i = 0; i < length; i++)
    {
        ListAddBegining(lst, values[i]);
    }
}
//Insère un nouvelle élément à l'index en param
void ListInsert(List* lst, int index, char value)
{
    if(index == 0)
    {
        ListAddBegining(lst, value);
        return;
    }
    Item* newItem = NewItem(value);
    Item* cursor = lst->root;
    int count = 0;
    while(count < index - 1 && cursor->following != NULL)
    {
        count++;
        cursor = cursor->following;
    }
    Item* tmp = cursor->following;
    cursor->following = newItem;
    newItem->following = tmp;
    lst->count++;
}
//private
void RemoveItem(Item* item)
{
    free(item);
}
//Supprime le premier élément de la liste
void ListRemoveBeg(List* lst)
{
    if(lst == NULL || lst->root == NULL)
        return;
    Item* toRm = lst->root;
    lst->root = lst->root->following;
    RemoveItem(toRm);
    lst->count--;
}
//supprime le dernier élément de la liste
void ListRemoveEnd(List* lst)
{
    if(lst == NULL || lst->root == NULL)
        return;
    if(lst->root->following == NULL)
    {
        RemoveItem(lst->root);
        lst->root = NULL;
        lst->count = 0;
        return;
    }
    Item* cursor = lst->root->following;
    Item* previousCursor = lst->root;
    while(cursor->following != NULL)
    {
        previousCursor = cursor;
        cursor = cursor->following;
    }
    RemoveItem(cursor);
    previousCursor->following = NULL;
}
//Supprime l'élément de la liste à l'index indiqué en param
void ListRemoveAt(List* lst, int index)
{
    if(index < 0 || index >= lst->count)
        return;
    if(index == 0)
    {
        ListRemoveBeg(lst);
        return;
    }
    
    int count = 0;
    Item* cursor = lst->root;
    while(count < index - 1 && cursor->following != NULL)
    {
        count++;
        cursor = cursor->following;
    }
    if(cursor->following != NULL)
    {
        Item* toRm = cursor->following;
        cursor->following = toRm->following;
        RemoveItem(toRm);
        lst->count--;
    }
}
//Affiche la liste dans la console sous la forme d'un tableau
void ListShow(List* lst)
{
    if(lst == NULL || lst->root == NULL)
    {
        printf("[]\n");
        return;
    }
    if(lst->root->following == NULL)
    {
        printf("[%c]\n", lst->root->value);
        return;
    }
    Item* cursor = lst->root;
    printf("[%c,", cursor->value);
    while (cursor->following->following != NULL)
    {
        cursor = cursor->following;
        printf("%c,", cursor->value);
    }
    printf("%c]\n", cursor->following->value);
}
//Renvoie un tableau décrivant la liste
int* ListToArray(List* lst)
{
    if(lst == NULL || lst->root == NULL)
        return NULL;
    char* res = (char*)malloc(lst->count * sizeof(char));
    Item* cursor = lst->root;    
    for (int i = 0; i < lst->count; i++)
    {
        res[i] = cursor->value;
        cursor = cursor->following;
    }
    return res;
}
//Ecrit la liste dans un fichier texte sous la forme d'un tableau
void ListWrite(List* lst, char* fileName)
{
    WriteArray(ListToArray(lst), lst->count, fileName);
}
//Libère la mémoire en supprimant la liste
void ListRemove(List* lst)
{
    while (lst->root != NULL)
    {
        ListRemoveBeg(lst);
    }
    free(lst);
}
//Renvoie la valeur de l'élément à l'index en param, si l'indice est incorecte renvoie NULL
int ListGetValue(List* lst, int index)
{
    if(lst == NULL || lst->root == NULL)
    {
        printf("Trying to acces a null list!\n");
        return 0;
    }
    if(index < 0 || index >= lst->count)
    {
        printf("Index exeption : %i\n", index);
        return 0;
    }
    int count = 0;
    Item* cursor = lst->root;
    while (count < index && cursor->following != NULL)
    {
        cursor = cursor->following;
        count++;
    }
    if(count == index)
    {
        return cursor->value;
    }
    printf("Index exeption : %i\n", index);
    return cursor->value;
}
//private
Item* GetItemOf(List* lst, int index)
{
    if(lst == NULL || lst->root == NULL)
        return NULL;
    if(index < 0 || index >= lst->count)
        return NULL;
    int count = 0;
    Item* cursor = lst->root;
    while (count < index && cursor->following != NULL)
    {
        cursor = cursor->following;
        count++;
    }
    if(count == index)
        return cursor;
    return NULL;
}
//Renvoie le premier index de la liste tel que l'élément a cette index à la value en param, si aucun élément n'a cette valeur renvoie -1
int ListGetIndexOf(List* lst, char value)
{
    if(lst == NULL || lst->root == NULL)
        return -1;
    int count = 0;
    Item* cursor = lst->root;
    while (cursor->value != value && cursor->following != NULL)
    {
        cursor = cursor->following;
        count++;
    }
    return cursor->value == value ? count : -1;
}
//private
int VerifyLength(List* lst)
{
    if(lst == NULL || lst->root == NULL)
        return 0;
    Item* cursor = lst->root;
    int count = 1;
    while (cursor->following != NULL)
    {
        count++;
        cursor = cursor->following;
    }
    return count;
}

//Inverse les élément au index 1 et 2 de la liste
void ListSwitch(List* lst, int index1, int index2)
{
    if(index1 < 0 || index2 < 0 || index1 >= lst->count || index2 >= lst->count)
        return;
    if(index1 > index2)
    {
        ListSwitch(lst, index2, index1);
        return;
    }
    if(index1 == index2)
        return;
    if(abs(index1 - index2) == 1)
    {
        if(index1 == 0)
        {
            Item* i1 = lst->root;
            Item* i2 = i1->following;
            i1->following = i2->following;
            i2->following = i1;
            lst->root = i2;
            return;
        }
        Item* pi1 = GetItemOf(lst, index1 - 1);
        Item* i2 = pi1->following->following;
        pi1->following->following = i2->following;
        i2->following = pi1->following;
        pi1->following = i2;
        return;
    }
    if(index1 == 0)
    {
        Item* tmp = lst->root->following;
        Item* pi2 = GetItemOf(lst, index2 - 1);
        Item* nr = pi2->following;
        lst->root->following = nr->following;
        nr->following = tmp;
        pi2->following = lst->root;
        lst->root = nr;
        return;
    }
    Item* pi1 = GetItemOf(lst, index1 - 1);
    Item* i1 = pi1->following;
    Item* pi2 = GetItemOf(lst, index2 - 1);
    Item* tmp = pi1->following->following;
    pi1->following->following = pi2->following->following;
    pi2->following->following = tmp;
    pi1->following = pi2->following;
    pi2->following = i1;
}

bool ListIsEmpty(List* lst)
{
    return lst->count == 0;
}

#endif