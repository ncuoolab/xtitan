package edu.ncu.csie.oolab;

import java.util.HashMap;

import org.python.core.PyException;
import org.python.util.PythonInterpreter;
import org.sikuli.script.FindFailed;

public class ExecuteCommand extends AbstractCommand {

	public ExecuteCommand( PythonInterpreter python ) {
		this.python_ = python;
	}

	@Override
	public HashMap< String, Object > execute( HashMap< String, Object > args ) {
		HashMap< String, Object > result = new HashMap< String, Object >();
		String line = ( String ) args.get( "script" );
		try {
			this.python_.exec( line );
		} catch( PyException e ) {
			result.put( "result", "failed" );

			Object type = e.type.__tojava__( Class.class );
			if( type.equals( FindFailed.class ) ) {
				FindFailed ff = ( FindFailed ) e.value.__tojava__( FindFailed.class );
				result.put( "type", "FindFailed" );
				result.put( "message", ff.getMessage() );
			} else {
				result.put( "type", "Unknown" );
				result.put( "message", e.value.toString() );
			}

			return result;
		}
		result.put( "result", "succeed" );
		return result;
	}

	private PythonInterpreter python_;

}
