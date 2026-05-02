#include "GameActor.h"

void GameActor::Heal(float _value)
{
	currentHealth += _value;
	currentHealth = gce::Min(currentHealth, maxHealth);
}

void GameActor::TakeDamage(float damageValue) {
	float damageTaken;
	if (armor > -99) {
		damageTaken = damageValue / (1.0f + armor / 100.0f);
	}
	else {
		damageTaken = damageValue / (1.0f - 99.0f / 100.0f);
	}
	currentHealth -= damageTaken;

	std::cout << "Damage taken : " << damageTaken << std::endl;
	if (currentHealth <= 0) {
		Death();
	}
}
