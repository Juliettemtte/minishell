<h1 align="center">
	minishell
</h1>
<p align="center">
	<b><i>As beautiful as a shell... </i></b><br>
</p>

### The Challenge
The goal was to create a mini version of bash, I say mini but it was still a huge project.  We had to learn how bash takes arguments, parses them, and executes them. We had to incorporate the following features:
- Display a promt while waiting for a new commmand.
- Have a working history.
- Find and launch executables (using the `PATH` variable or an absolute path).
- Handle `'` (single) and `"` (double) **quotes** like in bash.
- Implement **redirections** such as:
  - `<` redirect input.
  - `>` redirect output.
  - `<<` heredoc (doesn't affect history).
  - `>>` redirect output in append mode.
- Implement `|` (**pipes**).
- Handle **environment variables**.
- Handle `$?`.
- `ctrl-C`, `ctrl-D`, and `ctrl-\` should behave like in bash.
- Recreate the following **builtins**:
  - `echo` with option `-n`.
  - `cd` with only a relative or absolute path.
  - `pwd` (no flags).
  - `export` (no flags).
  - `unset` (no flags).
  - `env` (no flags or arguments).
  - `exit` (no flags).

We didn't have to implement && and ||, or wildcards, as well as any special symbols that weren't specifically asked for.  We were also told that when in doubt, take bash as a reference.
This sometimes led to discussions on wether or not we had to implement something.

### Installation
#### Clone the repository:
``` 
git clone https://github.com/Juliettemtte/minishell.git
cd minishell
make
```
#### Run Minishell
```
./minishell
```

#### Some commands to try:

As this project is made to mimic bash, you can try any commands you normally would try in bash.

If you really can't think of anything try some of the following.  You do have to enter each line separately as the program doesn't handle new lines.
```
cat minishell.c | grep int | wc -l
```

```
echo "Hello" > file.txt
cat < file.txt
ls nonexistent_dir 2> error.log
```

```
ls | rev > file
cat file
rev file | cat
rm file
```

*to exit the program:*
```
exit
```
------
<p align="center">
	Made by Anssi RISSANEN (@arisane) and Juliette MOUETTE (@jmouette) <br>
</p>
