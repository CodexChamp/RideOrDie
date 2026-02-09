# Green Team
GAM-305-12908-M01 Digital Game Development

## Module Two Team Project Plan
**Contributors:** Rylan, Zachary, Devin, Travisk, Kenan*

### Team Roles
* **Team Lead:** Rylan
* **Artist:** Kenan
* **Programmer:** Devin / Travis
* **UI/UX Programmer/Designer:** Rylan
* **Level Designer/World Builder:** Zack / Travis

---

### 1. Scenario & Additional Elements
**Scenario:** Third-Person Game (Theme: Vehicle Zombie Survival)

**Rationale:** We are using the Unreal Third-Person template but replacing the character with a car to create a survival driving game.

**The Four Additional Elements:**
* **Vehicle Physics & Combat:** Using the car as a weapon to run over zombies (and taking damage from them).
* **Horde AI Logic:** Simple AI that makes zombies swarm toward the player's position.
* **Resource/Pickup System:** Collecting fuel or repair kits to keep the run going.
* **Win/Loss State UI:** "Car Destroyed" screen (Loss)\ A zombies killed count until all zombies killed or a survival timer (Win)

### 2. Brainstorming on Content (Assets)
We brainstormed the specific assets needed to make that scenario work:
* **Art:** Vehicle model, Zombie character model, City/Highway environment assets (roads, barriers), UI elements (Health bar, Fuel gauge).
* **Audio:** Engine revving, impact sounds (hitting zombies), background ambient track, game over sound effects.
* **Code:** Vehicle controller script, Enemy navigation (NavMesh), Game Loop Manager (Spawn logic, Win/Loss conditions).

### 3. Development Schedule & Timeline
* **Week 2 – Planning & Setup**
  * Finalize game concept and scope (top-down survival)
  * Assign roles and responsibilities
  * Set up repo, engine/project, basic folder structure
  * Draft high-level design document
* **Week 3 – Core Systems (Alpha Start)**
  * Player movement and camera
  * Basic enemy AI and spawning
  * Core combat or interaction mechanic
  * Placeholder UI (health, basic HUD)
* **Week 4 – Alpha Build**
  * Core gameplay loop playable
  * One complete level or area
  * Basic win/lose condition
  * **Alpha Submission**
* **Week 5 – Feature Expansion (Beta Start)**
  * Additional enemies or mechanics
  * Improved UI/UX pass
  * Sound effects and basic music
  * Bug fixes from Alpha feedback
* **Week 6 – Beta Build**
  * Full playable experience end-to-end
  * Balance tuning
  * Visual polish pass
  * **Beta Submission**
* **Week 7 – Final Polish & Submission**
  * Final bug fixes
  * Performance cleanup
  * Final UI/UX tweaks
  * Final build and documentation submission

### 4. Alpha vs. Beta Development Goals

**Alpha Stage (Functionality Focus)**
* **Assets:** Grey-box environment and placeholder models for car and zombies.
* **Functionality:**
  * Car drives without physics errors.
  * Zombies spawn and track the player.
  * Basic collision logic (hitting zombies works).
* **Tracking:** Core mechanics are feature-complete.

**Beta Stage (Polish Focus)**
* **Assets:** Final meshes, textures, and UI art implemented. Audio (SFX/Music) added.
* **Functionality:**
  * Win/Loss screens fully working.
  * Handling tuned for better game feel.
  * Performance optimized.
* **Tracking:** Content lock (no new features), focus on bug fixing.

---

### 5. Communication Method
* **Discord**

### 6. Meeting Plans
* **TBD**

### 7. Task Assignment & Reporting
* **Method:** Google Doc Traceability Matrix

## Module Three Project Log - Team Development: QA and Testing Plan.
**Contributors:** Rylan, Zachary, Devin, Travisk, Kenan*

How we are testing everything Instead of waiting until the end, we are going to test things as we make them. We want to make sure individual parts (like the car physics or a specific zombie) actually work on their own before we try to put them into the main level.

Weekly Schedule

Monday – Thursday (Building): We work on our assigned features and test them on our own computers first to make sure they aren't totally broken.

Friday (Putting it together): Everyone uploads their work to the main project files. We make a "build" of the game so everyone has the same version.

Saturday/Sunday (Test Day): The whole team plays the game together. We look for bugs, see if the game is actually fun, and check if the new stuff works.

The 3 Main Testing Stages

Play Test (Early on): We are testing the "grey-box" version just to see if the movement and zombies feel good. We aren't worried about graphics yet, just gameplay.

Code Release (During development): Every week we check the Friday build against our list to make sure the new code didn't break the old stuff (regression testing).

Demo (Near the end): We will test a polished "Vertical Slice" of the game to make sure it doesn't crash while we are showing it off to people.

Test Item Checklist

Controls

What to look for: Do the keys (WASD) and controller work instantly without lag?

Pass/Fail: [ ]

Logic

What to look for: Does the game actually know when you win or die?

Pass/Fail: [ ]

Game Loop

What to look for: Can you Start -> Play -> Die -> and Restart without it crashing?

Pass/Fail: [ ]

Mechanics

What to look for: Do powerups work? Does the car drive right or does it flip over too much?

Pass/Fail: [ ]

Performance

What to look for: Does the game lag when there are a lot of zombies?

Pass/Fail: [ ]

Tracking Bugs & Changes

Updating the Plan: We have meetings on Wednesdays and Saturdays. If we decide to cut a feature or change how something works (like how fast the car goes), we will update our Traceability Matrix so we don't forget.

Reporting Bugs: When we find bugs during our Saturday playtests, we will write them down in the Matrix. We'll note how bad the bug is and how to make it happen again.

Tracking Fixes: We will use the matrix to see what's still broken. Once a bug is fixed in the next Friday build, we will mark it as "Resolved."


## Module Four Project Log – Team Reflection
**Contributors:** Rylan, Zachary, Devin, Travisk, Kenan*

**What parts of the testing process did the team perceive to go well?**  
When testing was possible, testing individual features in isolation worked well. Small systems like basic vehicle movement, UI elements, and simple logic could be tested quickly using Unreal’s Play-In-Editor tools. Informal playtesting helped identify obvious issues early when features were available to test.

**How were bugs identified and corrected?**  
Bugs were identified primarily through hands-on testing and playtesting. When issues were found, they were discussed by the team and documented so they could be addressed later. However, many fixes were delayed due to repository issues rather than technical difficulty with the bugs themselves.

**In terms of the QA and testing process, what would you do differently to improve the process?**  
A major challenge during this module was that the team’s GitHub repository ran out of Git LFS bandwidth. This prevented assets from being downloaded and blocked development for much of the week. As a result, a large portion of the team’s time was spent troubleshooting repository and asset management issues instead of actively developing and testing the game. In the future, we would verify repository configuration and asset storage limits earlier to avoid blocking development during critical milestones.

**What tools (chosen in Module Two) did you find successful in the development of your Alpha project? Why?**  
Discord and GitHub were still effective tools for communication and coordination. Discord allowed the team to quickly communicate problems and updates, while GitHub helped manage project structure and documentation. The traceability matrix was also helpful for keeping track of completed tasks and known issues, especially while development progress was slowed.

**Were there any tools or techniques that you did not find helpful in the success of your project development? Why?**  
Some third-party assets introduced unnecessary complexity due to underlying code and dependencies that were not immediately clear. Additionally, reliance on Git LFS without sufficient bandwidth caused significant delays. These issues limited the team’s ability to test and iterate on gameplay during this module.

**How did the team approach to the initial analysis of the game design document contribute to the decision to use these tools and techniques?**  
The initial analysis of the game design document helped the team focus on core gameplay systems and choose tools that supported rapid prototyping and iteration. While external issues limited progress during this module, the original planning helped the team stay aligned on priorities and identify what features needed to be completed once development could resume.

## Module Five Project Log – Team Reflection
**Contributors:** Rylan, Zachary, Devin, Travisk*

What parts of the plan did the team perceive to go well in relation to the last stage evaluation?
One thing that went well was how the team responded when a major issue occurred. After the repository broke, the team worked together to identify the cause, troubleshoot the problem, and restore the project so development could continue. This showed that our communication and problem-solving process worked even under pressure.

What parts of the plan did the team perceive to go wrong in relation to the last stage evaluation?
What did not go well was the introduction of C++ and Visual Studio project files into the repository. This caused compatibility issues and prevented some team members from opening or running the project. As a result, a large portion of the week was spent fixing repository and environment problems instead of working on gameplay features.

Previous evaluations were still useful and were integrated into this stage. We made design changes such as adding the need for fuel and adjusting gameplay systems based on earlier feedback. We also realized the importance of everyone using the same software versions. While that caused some short-term issues, it helped stabilize the project once everyone was aligned.

If we were to improve the collaboration and development process, we would be much stricter about using branches and avoiding direct work in the main branch. That caused several problems and made merging harder than it needed to be. We also noticed that roles started to overlap too much at times, with people stepping into areas outside their assigned responsibilities. While that came from trying to help, it sometimes created confusion instead of progress.

Some tools and techniques were less helpful than expected. Extra Fab assets and folders added unnecessary files to the repository, which contributed to our Git LFS bandwidth issues. While the assets themselves were useful, bringing in too many at once created more problems than benefits and slowed the team down.

As we move toward the Beta stage and final release, our focus is on getting everything working cleanly and cohesively. This includes finishing the UI elements such as the leaderboard and main menu, improving zombie actions so things like dying animations look better, making power-up selection feel intuitive, and ensuring the full game loop works from start to finish without breaking. The goal now is stability, clarity, and polish instead of adding new features at the last minute.
