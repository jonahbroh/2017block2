package ch.idsia.scenarios;

import ch.idsia.ai.agents.Agent;
import ch.idsia.ai.agents.AgentsPool;
import ch.idsia.ai.agents.human.HumanKeyboardAgent;
import ch.idsia.ai.tasks.ProgressTask;
import ch.idsia.ai.tasks.Task;
import ch.idsia.tools.CmdLineOptions;
import ch.idsia.tools.EvaluationOptions;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
/**
 * Created by IntelliJ IDEA.
 * User: julian
 * Date: May 5, 2009
 * Time: 12:46:43 PM
 */
public class Play {

    public static void main(String[] args) {
        Agent controller = new HumanKeyboardAgent();
        if (args.length > 1) {
            controller = AgentsPool.load (args[0], args[2]);
            AgentsPool.addAgent(controller, args[2]);
        }
        EvaluationOptions options = new CmdLineOptions(new String[0]);
        options.setAgent(controller);
        Task task = new ProgressTask(options);
        options.setMaxFPS(false);
        options.setVisualization(true);
        options.setNumberOfTrials(1);
        options.setMatlabFileName("");
        options.setLevelRandSeed((int) (Math.random () * Integer.MAX_VALUE));
        options.setLevelDifficulty(3);
        task.setOptions(options);
        if (args.length > 1){
          try{
            BufferedWriter bw = null;
  		      FileWriter fw = null;
            String filename = "./scores/fitness" + args[1];
            File file = new File(filename);
            if (!file.exists()){
              file.createNewFile();
            }

            fw = new FileWriter(file.getAbsoluteFile());
  			    bw = new BufferedWriter(fw);

            bw.write(Double.toString(task.evaluate(controller)[0]));
            bw.close();
            fw.close();
          }catch(IOException e){
            System.out.println("PROBLEM");
          }
        }
        else{
          System.out.println ("Score: " + task.evaluate (controller)[0]);
        }
    }
}
