# AimAssistPackage

## Playable Character
### Variables
<ins>Aim Assist</ins>
- Actors to Ignore: List of actors to be ignore with the aim assist
- Actors Type to Aim: List of type of actors who has the ability to trigger the aim assist

- High Priority Component: name of the most important component name, used in case there is multiple components, for exemple, head and body.
- Component Swap Treshold: Paired with the previous variable, in order to reward the player for his good aim, help him to target the High priority component, even if he's not on the component anymore but on an other one. It's the treshold to swap between both;
- Sensibility: Define the speed of the help
- Speed Rotation Treshold: trigger the Aim assist only if the the player is moving his mouse above this limit. Used to prevent the auto target even if the player isn't moving its mouse
- Max Range: Max range of the aim assist
- Capsule Radius: Radius of the aim assist


<ins>Statistics</ins>
- MaxHealthPoint: Use to define the life of the player

### Functions
- Detection: Use to know if the aim assist detect an actor. Return a bool.
- IsAimAssistEnable: Check if the speed rotation of the player is above its treshold.
- SelectComponent: If targeted component changed, check with the previous one to define the which one has the priority over the other and retun the chosen one.
- AutoRotate: Change the rotation of the player, based on the Sensibility, in order to aim the targeted component

## Weapon
### Variables
- WeaponDamage: Determine the weapon damages
- FireSound: Sound to play on shoot
- FireAnimation: Animation to add on shoot to the player

### Functions
- AttachWeapon: Attach weapon on pick up
- Fire: shooting action

## Enemy
### Variables
<ins>Weapon</ins>
- FireSound: Sound played on shoot
- AimPercentage: Ratio of shot hit per shot fired
- Missing Offset: When the enemy miss the player, how much he miss

<ins>Statistics</ins>
- Damages: Base damages of the enemy
- HealthPointMax: Max life of the enemy

<ins>Damage Handling</ins>
- WeakZoneMultiplier: Multiplier if got hit in a weak point (e.g. the head)
- MidZoneMultiplier: Base damage multiplier (e.g. Torso)
- ResistantZoneMultiplier: Multiplier if got hit in an non vital zone (e.g. Legs)
- WeakZoneName: name of the component defining the weak point
- MidZoneName: name of the component defining the mid zone
- ResistantZoneName: name of the component defining the mid zone

### Functions
- DamageCalculator: Use to calcul the damage depending on the component hit;
- Firing: Shoot in the direction of the player
- GetFiringEndLocation: Determine where to shoot, if the shoot hit or miss;

