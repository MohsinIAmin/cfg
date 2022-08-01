package cfg;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class Driver {
	public static void main(String[] args) {
		String filePath = "hello.c";
		try {
			FileReader fileReader = new FileReader(filePath);
			BufferedReader bufferedReader = new BufferedReader(fileReader);
			String line;
			ArrayList<String> lines = new ArrayList<>();
			while((line = bufferedReader.readLine())!=null){
//				System.out.println(line);
				lines.add(line);
			}
            bufferedReader.close();
            fileReader.close();

            CFG cfg = new CFG(lines);
			cfg.graph();

		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}
}
