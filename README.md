# Project_3
## Kye Norcross and Emma Valente
### System Behavior:
In this project we built upon our design from project 2. The ignition system remained the same however we removed the headlight subsytem and
added a windshiled wiper subsystem. In order for the wipers to be utilized the ignition must be on, indicated by the blue LED as a part of the
ignition subsytem. Once on, the user can choose between Hi, Low, Off, or Int settings. In Hi the wipers move as intended with maximum speed
and in Low with minimal speed. While in Int mode the wipers will move at maximum speed with a desired pause in between cycles. This is selected
by the user in increments of 3, 6, or 8 seconds. Additionally, if the setting is changed to off or the system is turned off, the wipers will 
finish the cycle they are on and then return to the resting position. And if the wipers are in int mode and hesitating before being turned off 
they will remain stationary.

### Design Alternatives:
For our design we chose to impliment a positional servo motor rather than a continuous servo for our windshild wiper subsystem. We chose a 
positional over continuous motor for its ability to more easily stop the motor at a desired point. This allowed for us to easily create the 
range of motion needed for a windshiled wiper. Furthermore with a positional servo motor, to achieve different rotational speeds, we 
only had to modify the pulse width modulation delays to a desired increment. This allowed for an easy distinction in motor speeds for the different wiper modes.

### Summery of Testing Results:
<img width="736" alt="Screenshot 2025-02-20 at 1 30 53 AM" src="https://github.com/user-attachments/assets/9c8f08cf-f138-4bad-820f-d8f9b3062791" />
<img width="712" alt="Screenshot 2025-02-20 at 1 33 37 AM" src="https://github.com/user-attachments/assets/9478c0dc-4aa1-41c9-8344-94fb4a018ffe" />
