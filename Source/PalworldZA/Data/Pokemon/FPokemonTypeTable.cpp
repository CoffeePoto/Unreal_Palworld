// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Pokemon/FPokemonTypeTable.h"

/*
NONE,
	NORMAL,
	FIRE,
	GRASS,
	GROUND,
	ELECTRICITY,
	WATER,
	ICE,
	DRAGON,
	DARK,
	COUNT
*/

// Attack Defence
const float FPokemonTypeTable::Table[TYPE_COUNT][TYPE_COUNT] =
{
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, /*NONE*/
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, /*NORMAL*/
	{1.0f, 1.0f, 0.5f, 2.0f, 1.0f, 1.0f, 0.5f, 2.0f, 1.0f, 1.0f}, /*FIRE*/
	{1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, /*GRASS*/
	{1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f}, /*GROUND*/
	{1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 1.0f, 1.0f, 1.0f}, /*ELECTRICITY*/
	{1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f}, /*WATER*/
	{1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 2.0f, 1.0f}, /*ICE*/
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 2.0f}, /*DRAGON*/
	{1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f}, /*DARK*/
};