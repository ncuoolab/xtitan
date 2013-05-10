package edu.ncu.csie.oolab;

import java.io.File;
import java.util.HashMap;

import org.python.core.PyException;
import org.python.util.PythonInterpreter;
import org.sikuli.script.FindFailed;
import org.sikuli.script.Settings;

public class BundleCommand extends AbstractCommand {

	public BundleCommand( PythonInterpreter python ) {
		this.python_ = python;
	}

	@Override
	public HashMap< String, Object > execute( HashMap< String, Object > args ) {
		String path = ( String ) args.get( "path" );
		HashMap< String, Object > result = new HashMap< String, Object >();

		Settings.BundlePath = path;

		try {
			this.python_.execfile( this.getPyFrom( path ) );
		} catch( PyException e ) {
			result.put( "success", false );

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
		} finally {
			Settings.BundlePath = null;
		}

		result.put( "success", true );
		return result;
	}

	private String getPyFrom( String path ) {
		String name = ( new File( path ) ).getName();
		String prefix = name.substring( 0, name.indexOf( '.' ) );
		return path + "/" + prefix + ".py";
	}

	private PythonInterpreter python_;

}
