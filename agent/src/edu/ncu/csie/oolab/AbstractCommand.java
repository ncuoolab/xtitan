package edu.ncu.csie.oolab;

import java.util.HashMap;

public abstract class AbstractCommand {

	public abstract HashMap< String, Object > execute( HashMap< String, Object > args );

}
