package ch.idsia.ai.agents.ai;

import ch.idsia.ai.agents.Agent;
import ch.idsia.ai.agents.ai.BasicAIAgent;
import ch.idsia.mario.engine.sprites.Mario;
import ch.idsia.mario.environments.Environment;

public class GAgent extends BasicAIAgent implements Agent
{
	public byte[][] levelScene;
	public byte[][] enemies;
	public int[][] transitions = new int[4][7];
	public int state = 0;

	public GAgent(String chromosome)
	{
		super("GAgent");
		reset();
		for(int i = 0; i < 7; i++){
			transitions[0][i] = chromosome.charAt(i);
		}
		for(int i = 7; i < 14; i++){
			transitions[1][i] = chromosome.charAt(i);
		}
		for(int i = 14; i < 21; i++){
			transitions[2][i] = chromosome.charAt(i);
		}
		for(int i = 21; i < 28; i++){
			transitions[3][i] = chromosome.charAt(i);
		}
	}

	// Does (row, col) contain an enemy?
	public boolean hasEnemy(int row, int col) {
		return enemies[row][col] != 0;
	}

	// Is (row, col) empty?
	public boolean isEmpty(int row, int col) {
		return (levelScene[row][col] == 0);
	}

	// Is (row, col) specifically a wall?
	public boolean isWall(int row, int col) {
		if (levelScene[row][col]  == 1 || levelScene[row][col]  == -60)
			return true;
		else
			return false;
	}

	public boolean isEnemy(){
		for (int r = 9; r<=13; r++){
			if (hasEnemy(9,r))
				return true;
		}
		return false;
	}

	public boolean isObstacle(){
		for (int r = 9; r<=13; r++){
			if (isWall(9,r))
				return true;
		}
		return false;
	}

	public boolean isHole(){
		if ((isEmpty(10,10) || isEmpty(10,9)))
		{
			return true;
		}
		return false;
	}

	public int getState(Environment environment){
		levelScene = environment.getLevelSceneObservationZ(2);
		enemies = environment.getEnemiesObservationZ(2);
		int status;
		boolean enemy = isEnemy();
		boolean obstacle = isObstacle();
		boolean hole = isHole();
		if(hole && obstacle){
			status = 6;
		}
		else if (enemy && obstacle){
			status = 5;
		}
		else if (enemy && hole){
			status = 4;
		}
		else if (hole){
			status = 3;
		}
		else if (obstacle){
			status = 2;
		}
		else if (enemy){
			status = 1;
		}
		else{
			status = 0;
		}
		return transitions[state][status];
	}

	// Actually perform an action by setting a slot in the action array to be true
	public boolean[] getAction(Environment environment)
	{
		state = getState(environment);
		if(state == 0){
			action[Mario.KEY_RIGHT] = true;
			action[Mario.KEY_JUMP] = action[Mario.KEY_SPEED] = false;
		}
		else if(state == 1){
			action[Mario.KEY_RIGHT] = action[Mario.KEY_JUMP] = true;
			action[Mario.KEY_SPEED] = false;
		}
		else if(state == 2){
			action[Mario.KEY_RIGHT] = action[Mario.KEY_SPEED] = true;
			action[Mario.KEY_JUMP] = false;
		}
		else if(state == 3){
			action[Mario.KEY_RIGHT] = action[Mario.KEY_SPEED] = action[Mario.KEY_JUMP] = true;
		}

		return action;
	}

	// // Do the processing necessary to make decisions in getAction
	// public void integrateObservation(Environment environment)
	// {
	// 	super.integrateObservation(environment);
  //   	levelScene = environment.getLevelSceneObservationZ(2);
	// }

	// Clear out old actions by creating a new action array
	public void reset()
	{
		state = 0;
	}
}
