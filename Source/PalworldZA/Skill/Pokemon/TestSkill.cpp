// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Pokemon/TestSkill.h"
#include "Interface/PokemonInterface/PokemonDataGetter.h"

ATestSkill::ATestSkill()
{

}

void ATestSkill::ExecuteSkill()
{
	
	IPokemonDataGetter* Getter = Cast<IPokemonDataGetter>(User);
	AActor* Target = Getter->GetTarget();

	UE_LOG(LogTemp, Log, TEXT("테스트 스킬"));
}

void ATestSkill::OnEndSkill()
{

}
