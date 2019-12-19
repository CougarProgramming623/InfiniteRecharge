# InfiniteRecharge
<p align="left">
    <a href="https://github.com/CougarProgramming623/InfiniteRecharge/graphs/contributors" alt="Contributors">
        <img src="https://img.shields.io/github/contributors/CougarProgramming623/InfiniteRecharge?color=Burgundy&style=for-the-badge&logo=GitHub" /> <img src="https://img.shields.io/github/issues/CougarProgramming623/InfiniteRecharge?logo=GitHub&style=for-the-badge" /> <img src="https://img.shields.io/github/followers/CougarProgramming623?color=bright-green&logo=GitHub&style=for-the-badge" /> <img src="https://img.shields.io/github/watchers/CougarProgramming623/InfiniteRecharge?color=bright-green&logo=GitHub&style=for-the-badge" /> <img src="https://img.shields.io/github/last-commit/CougarProgramming623/InfiniteRecharge?logo=GitHub&style=for-the-badge" /> </a>

## Github Information
Here you can find defintions for basic GitHub terminology as well as examples

`Branch`
A version of the repository that diverges from the main working project. Branches can be a new version of a repository, experimental changes, or personal forks of a repository for users to alter and test changes.

`Checkout`
The git checkout command is used to switch branches in a repository. git checkout testing-el would take you to the testing-el branch whereas git checkout master would drop you back into master. Be careful with your staged files and commits when switching between branches.

`Cherry-picking`
When cherry-picking a commit in Git, you are taking an older commit, and rerunning it at a defined location. Git copies the changes from the original commit, and then adds them to the current location.

`Clone`
A clone is a copy of a repository or the action of copying a repository. When cloning a repository into another branch, the new branch becomes a remote-tracking branch that can talk upstream to its origin branch (via pushes, pulls, and fetches).

`Fetch`
By performing a Git fetch, you are downloading and copying that branch’s files to your workstation. Multiple branches can be fetched at once, and you can rename the branches when running the command to suit your needs.

`Fork`
Creates a copy of a repository.

`HEAD`
HEAD is a reference variable used to denote the most current commit of the repository in which you are working. When you add a new commit, HEAD will then become that new commit.

`Index`
The working, or staging, area of Git. Files that have been changed, added and deleted will be staged within the index until you are ready to commit the files. To see what is set in your Git index, run git status within your repository. The green files are staged and ready to commit, whereas the red files have not yet been added to staging for the next commit.

`Master`
The primary branch of all repositories. All committed and accepted changes should be on the master branch. You can work directly from the master branch, or create other branches.

`Merge`
Taking the changes from one branch and adding them into another (traditionally master) branch. These commits are usually first requested via pull request before being merged by a project maintainer.

`Origin`
The conventional name for the primary version of a repository. Git also uses origin as a system alias for pushing and fetching data to and from the primary branch. For example, git push origin master, when run on a remote, will push the changes to the master branch of the primary repository database.

`Pull/Pull` Request
If someone has changed code on a separate branch of a project and wants it to be reviewed to add to the master branch, that someone can put in a pull request. Pull requests ask the repo maintainers to review the commits made, and then, if acceptable, merge the changes upstream. A pull happens when adding the changes to the master branch.

`Push`
Updates a remote branch with the commits made to the current branch. You are literally “pushing” your changes onto the remote.

`Rebase`
When rebasing a git commit, you can split the commit, move it, squash it if unwanted, or effectively combine two branches that have diverged from one another.

`Remote`
A copy of the original branch. When you clone a branch, that new branch is a remote, or clone. Remotes can talk to the origin branch, as well as other remotes for the repository, to make communication between working branches easier.

`Repository` (“Repo”)
In many ways, you can think of a Git repository as a directory that stores all the files, folders, and content needed for your project. What it actually is, is the object database of the project, storing everything from the files themselves, to the versions of those files, commits, deletions, et cetera. Repositories are not limited by user, and can be shared and copied (see: fork).

`Stash`
While working with Git, you may need to make multiple changes to files, but you may not want all changes to go in one commit. If you want to pause the changes you are working on now in favor of working on another issue or improvement, you can “stash” your changes, essentially clearing them from the staging area until the changes are called again. You can only stash one set of changes at a time. To stash your staging area use git stash [files]; to retrieve the stashed files, run git stash pop. You can also clear the stashed files with git stash drop.

`Tag`
Tags are used to define which portions of a project’s Git history is most important. Often this is used to note point releases of a project. Tags can be added to the commit you are working with or added after-the-fact when needed.

`Upstream`
While there is not necessarily a default “upstream” or “downstream” for Git projects, upstream can be considered where you push your Git changes — this is often the master branch of the project within the origin
### GitFlow
The purpose of GitFlow is to streamline production of code. There are two main types of branches we use: Feature Branches and the Master Branch. Code is worked on each branch independently. All "features" of the robot such as modules and harware requirments will have their own branch. Once the code is completed and stable, it will be merged into `master`. All competition ready code will be tagged for use.
## Rules
### General Rules
1. Be respectful and friendly
2. Complete assignments on time and attend meetings consistently 
3. Communicate any questions and concerns with leads
4. Have fun!
5. Be safe 
### Github Rules
1. Use project board for outstanding tasks. If a new assignment comes up, add it to the project board under the correct category. All assignments, along with a timeline and priority, should appear on the board.
2. Use issues page in order to track progress. All problems currently being worked on should have their own thread.
3. Releases and tags should be used effectively. All working completed builds should have their own release (Deployable to Robot/ Test-Team), as well as any builds after substantial progress has been made.
4. Request code merge to the _correct branch_
5. When merging, gain the approval of at least one other software member before doing so. The code will then be sent to the lead for review automatically.
6. When pulling code, pull from tagged or released versions, as to not be working with a version behind the rest of the team. 
### Code Standards
1. All header files should begin with the header guard  `#pragma once`
2. Short functions (< 5 lines) should be marked as inline
3. Includes should have a newline between different sets of similar files
4. The `namespace ohs623` must  be used around any **LIBRARY CODE** and the `namespace frc2020` must be used around any code for **THIS YEAR**
5. Local variables must be declared and assigned in the same line
6. All primitive types should be passed to functions _by value_, everything else should be passed _by reference_
7. Use a **struct** only for passive objects that carry data; everything else should be made a **class**
8. **One** pointer type is allowed to be declared per line
9. Use `0` for integers, `0.0` for reals, `nullptr` for pointers, and `'\0'` for chars
10. Lower camel case is to be used when naming _variables_
11. Upper camel case is to be used when naming _classes, files, methods, and functions_ 
12. The class name should match with the file name
13. Macros and const variables should be uppercase with underscores
14. use `//` for comments **<= 2 lines** in series and `/*` for anything longer
15. Indentation is **only** to be done with tabs with the exception of lining up numbers and such across multiple lines
16. Opening curly brackets must be placed on the **same** line, with else and else if clauses on the **same** line as their curly brackets. However, starting and ending a pair of curly brackets on the same line is allowed _as long as nothing is inside_
17. Member variables in classes should include the prefix m_ and always be private. This is not necessary while using structs
18. In classes, a new visibility modifier (enter) is to be used to distinguish between methods and instance variables.
19. Grouping of code will go as follows: Also the order of groupings of things inside classes will go as follows:
   * Static variables (should be avoided)
   * Static methods
   * Public instance variables (should be not used in most cases (use getters and setters))
   * Public methods
   * Private methods
   * Private instance variables
20. Check with a lead if you are confused or need an example
## Sources
* [Linux Academy](https://linuxacademy.com/blog/linux/git-terms-explained/)
* [GitFlow](https://nvie.com/posts/a-successful-git-branching-model/) 

